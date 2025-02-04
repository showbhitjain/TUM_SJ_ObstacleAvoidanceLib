function [optimal_joint_velocity, Exit_Flag] = inverseKinematicsOAModified( ...
    joint_values,       ... % [n_joints x 1]
    jacobi,             ... % [6 x n_joints]
    xd_eff_vel,         ... % [6 x 1]
    jointminvalues,     ...
    jointmaxvalues,     ...
    joint_min_vel,      ...
    joint_max_vel,      ...
    J_g,                ... % Inequality constraint matrix
    b_g,                ... % Inequality constraint vector
    jointVelocityWeightMatrix, ...
    configInput)

    %----------------------------------------------------------------------
    % 1. PERSISTENT VARIABLES
    %----------------------------------------------------------------------
    persistent starting_joint_vel is_first_step 
    
    n_joints = length(joint_values);  % Number of joints
    n_slack  = 6;                     % One slack variable per end-effector velocity component (if used)
    
    % Initialize the persistent variables only on the first call
    if isempty(is_first_step) || isempty(starting_joint_vel)
        is_first_step = true;
        starting_joint_vel = zeros(n_joints,1); % Default
        
    end
    
    if is_first_step
        is_first_step = false;
    end

    % Unpack config for convenience
    config = configInput;
    
    %----------------------------------------------------------------------
    % 2. DETERMINE IF SLACK IS ACTUALLY USED
    %----------------------------------------------------------------------
    % The new dynamic logic:
    %   - If config.dynamicSlack is true AND (J_g or b_g are empty),
    %     we forcibly turn off slack.
    %   - Otherwise, we do whatever config.applySlack says.
    %
    slackIsUsed = config.applySlack;
    if config.dynamicSlack
        if isempty(J_g) || isempty(b_g)
            slackIsUsed = false;
        end
    end

    %----------------------------------------------------------------------
    % 3. BUILD VELOCITY DAMPER MATRICES IF NEEDED
    %----------------------------------------------------------------------
    In            = zeros(n_joints);        % Will hold diagonal matrix for damped joints
    bp            = zeros(n_joints,1);         % Will hold vector for velocity damper if needed
    infuence_dist = deg2rad(config.jointLimitActivationDistance);
    stop_dist     = deg2rad(config.jointLimitStopDistance);
    joint_gain    = config.jointLimitGain;
        
     if config.applyVelocityDamper
        
        % Compute distance to each joint limit, apply a linear velocity damper
        min_dist = min(abs(jointminvalues - joint_values), ...
                       abs(jointmaxvalues - joint_values));
        for i = 1:n_joints
            if min_dist(i) < infuence_dist
                In(i,i)  = 1;
                bp(i)    = joint_gain * (min_dist(i) - stop_dist)/(infuence_dist - stop_dist);
            end
        end
    end

    %----------------------------------------------------------------------
    % 4. SET UP CONSTRAINTS AND BOUNDS
    %----------------------------------------------------------------------
    % Default: no constraints
    Aeq = [];
    beq = [];
    A   = [];
    b   = [];
    lb  = [];
    ub  = [];
    
    % Weight matrix for Jacobian in equality constraints
    JacobiWeightMatrix = diag([1,1,1,1,1,1]); 
    
    % ---- 4.1 Equality constraints ---------------------------------------
    if config.applyEqualityConstraints
        Aeq = JacobiWeightMatrix * jacobi;
        beq = xd_eff_vel;
    end
    
    % ---- 4.2 Inequality constraints -------------------------------------
    if config.applyInequalityConstraints
        if ~isempty(J_g) && ~isempty(b_g)
            if config.applyVelocityDamper
                % Combine both standard inequalities and velocity damper
                A = [J_g; In];
                b = [b_g; bp];
            else
                % No velocity damper
                A = J_g;
                b = b_g;
            end
        else
            % If J_g, b_g are empty, we only apply velocity damper if set
            if config.applyVelocityDamper
                A = In;
                b = bp;
            end
        end
    else
        % If we do not apply inequality constraints at all but velocity
        % damper is on, we can still do that by adding A=In, b=bp.
        if config.applyVelocityDamper && (isempty(J_g) || isempty(b_g))
            A = In;
            b = bp;
        end
    end

    % ---- 4.3 Determine final use of Slack and build lb, ub --------------
    if ~slackIsUsed
        % BOUNDS without slack
        if config.applyVelocityDamper
            % We do not scale velocities by gamma if velocity damper is active
            lb = joint_min_vel;
            ub = joint_max_vel;
        else
            % Normal bounds: scale the move by config.gamma
            lb = max(config.gamma .* (jointminvalues - joint_values), joint_min_vel);
            ub = min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel);
        end
    else
        % BOUNDS with slack
        %    first n_joints are for joint velocities
        %    next   n_slack are for slack
        if config.applyVelocityDamper
            lb_joints = joint_min_vel;
            ub_joints = joint_max_vel;
        else
            lb_joints = max(config.gamma .* (jointminvalues - joint_values), joint_min_vel);
            ub_joints = min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel);
        end
        
        lb_slack = config.Slacklowerbound;
        ub_slack = config.Slackupperbound;
        lb       = [lb_joints; lb_slack];
        ub       = [ub_joints; ub_slack];
    end

    % ---- 4.4 Expand constraints for Slack if needed ----------------------
    if slackIsUsed
        % Expand the equality constraints
        if config.applyEqualityConstraints
            Aeq = [JacobiWeightMatrix * jacobi, eye(n_slack)];
            beq = xd_eff_vel; 
        end
        
        % Expand the inequality constraints
        if ~isempty(A)
            A = [A, zeros(size(A,1), n_slack)];
        end
    end

    %----------------------------------------------------------------------
    % 5. INITIAL GUESS
    %----------------------------------------------------------------------
    if slackIsUsed
        q_vel_initial_guess = [starting_joint_vel;zeros(n_slack,1)];
    else
        q_vel_initial_guess = starting_joint_vel;
    end

    %----------------------------------------------------------------------
    % 6. OPTIMIZATION OPTIONS
    %----------------------------------------------------------------------
    options = optimoptions('fmincon', ...
        'Algorithm',           'sqp', ...
        'Display',            'off', ...  % or 'iter' to see solver progress
        'OptimalityTolerance', 1e-6, ...
        'ConstraintTolerance', 1e-6, ...
        'StepTolerance',       1e-6, ...
        'MaxIterations',       1000);

    %----------------------------------------------------------------------
    % 7. CALL THE OPTIMIZER (fmincon)
    %----------------------------------------------------------------------
    [q_vel_opt, ~, Exit_Flag] = fmincon(@(q_vel) objFun( ...
            q_vel, ...
            jacobi, ...
            xd_eff_vel, ...
            starting_joint_vel, ...
            joint_values, ...
            jointVelocityWeightMatrix, ...
            config, ...
            slackIsUsed), ...
        q_vel_initial_guess, ...
        A, b, Aeq, beq, lb, ub, [], options);

    %----------------------------------------------------------------------
    % 8. EXTRACT THE RESULT
    %----------------------------------------------------------------------
    if slackIsUsed
        optimal_joint_velocity = q_vel_opt(1:n_joints);
        
    else
        optimal_joint_velocity = q_vel_opt;
    end

    %----------------------------------------------------------------------
    % 9. UPDATE PERSISTENT STATE
    %----------------------------------------------------------------------
    starting_joint_vel = optimal_joint_velocity;
end
