function [optimal_joint_velocity,Exit_Flag]  = inverseKinematicsOA(joint_values,jacobi,xd_eff_vel,jointminvalues,jointmaxvalues,joint_min_vel,joint_max_vel,J_g,b_g,jointVelocityWeightMatrix,configInput)
persistent starting_joint_vel is_first_step

n_joints = length(joint_values);  % Assuming 'joint_values' is a column vector
n_slack = 6;                       % Number of slack variables, one for each end-effector velocity component
%total_variables = n_joints + n_slack;  % Total number of variables in the optimization problem
config = configInput;
% Ensure sizes are compatible for code generation
%coder.varsize('starting_joint_vel', [n_joints, 1], [true, false]);
%coder.varsize('optimal_joint_velocity', [n_joints, 1], [true, false]);



% Initialize 'starting_joint_vel' if it's the first execution or if it's
% undefined (starting joint velocity)
if isempty(is_first_step) || isempty(starting_joint_vel)
    is_first_step = true;
    starting_joint_vel = zeros(n_joints,1); % Default initialization

end

% Then, check if it's the first execution step to perform any first-time setup
if is_first_step
    is_first_step = false; % Ensure this block won't execute again
    % No need to set 'starting_joint_vel' here again if it's already set above
end


JacobiWeightMatrix = diag([1 1 1 1 1 1]);
% delta = weights(1);
% alpha = weights(2);
% beta = weights(3);
% omega = weights(4);
    bp = zeros(n_joints,1);
    In = zeros(n_joints);

if config.applyVelocityDamper
    min_dist = min(abs(jointminvalues - joint_values),abs(jointmaxvalues-joint_values));
    infuence_dist = deg2rad(config.jointLimitActivationDistance);
    stop_dist = deg2rad(config.jointLimitStopDistance);
    joint_limit_gain = config.jointLimitGain;



    for i= 1:n_joints
        if min_dist(i) < infuence_dist
            In(i,i) = 1;
            bp(i) = joint_limit_gain * (min_dist(i) - stop_dist)/(infuence_dist - stop_dist) ;

        end
    end
    
end
% min_slack = [0.2,0.2,0.2,deg2rad(1),deg2rad(1),deg2rad(60)];
% max_slack = [-0.2,-0.2,-0.2,deg2rad(1),deg2rad(1),deg2rad(60)];
% %slack distance minimization
% slack_dist = [0.1,0.1,0.1,];


% Default initializations
Aeq = [];
beq = [];
A   = [];
b   = [];
lb = [];
ub = [];

% Check if equality constrjoint_valuesaints should be applied
if config.applyEqualityConstraints
    Aeq = JacobiWeightMatrix * jacobi;
    beq = xd_eff_vel;
end

% Check if inequality constraints should be applied
if config.applyInequalityConstraints

    if ~config.applyVelocityDamper
        A = J_g;
        b = b_g;
    end
    if config.applyVelocityDamper
        A = [J_g;In];
        b = [b_g;bp];
    end

end

if isempty(J_g) && isempty(b_g)
    Aeq = JacobiWeightMatrix * jacobi;
    beq = xd_eff_vel;
    % A = J_g;
    % b = b_g;
    if config.applyVelocityDamper
        A = In;
        b = bp;
    end
    config.applySlack = false ;
    % Define the objective function without slack variables
    %objective = @(q_vel) configurableObjective(q_vel, jacobi, xd_eff_vel, starting_joint_vel,joint_values, config);

    % Set bounds without slack variables
    if ~config.applyVelocityDamper
        lb = max(config.gamma .* (jointminvalues - joint_values), joint_min_vel); % Element-wise max
        ub = min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel); % Element-wise min
    end
    if config.applyVelocityDamper
        lb = joint_min_vel;
        ub = joint_max_vel;
    end
    % Initial guess without slack variables
    %q_vel_initial_guess = starting_joint_vel;
end

if config.applySlack
    % Define the augmented objective function that includes slack penalty
    % objective = @(q_vel) configurableObjective(q_vel(1:n_joints), jacobi, xd_eff_vel, starting_joint_vel, joint_values,config) ...
    %          + config.Slack_objective_weight* q_vel(n_joints+1:end)' * config.Slack_penalty_JacobiWeightMatrix * q_vel(n_joints+1:end) ;

    % Augment the bounds for the slack variables
    if ~config.applyVelocityDamper
        lb = [max(config.gamma .* (jointminvalues - joint_values), joint_min_vel); config.Slacklowerbound]; % Slack lower bounds
        ub = [min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel); config.Slackupperbound]; % Slack upper bounds
    end
    if config.applyVelocityDamper
        lb = [joint_min_vel;config.Slacklowerbound];
        ub = [joint_max_vel;config.Slackupperbound];
    end
    % Augment the equality constraint matrices for slack variables
    if config.applyEqualityConstraints

        Aeq = [JacobiWeightMatrix * jacobi, eye(n_slack)];
        beq = xd_eff_vel;  % Make sure this accounts for the slack if necessary

    end

    if config.applyInequalityConstraints
        num_rows = size(J_g,1);

        if config.applyVelocityDamper
            A = [[J_g, zeros(num_rows,n_slack)];[In,zeros(n_joints,n_slack)]];
            b = [b_g; bp];  % Make sure this accounts for the slack if necessary
        end
        if ~config.applyVelocityDamper
            A = [J_g, zeros(num_rows,n_slack)];
            b = b_g;
        end

    end
    % Initial guess should include the slack variables
    q_vel_initial_guess = [starting_joint_vel; zeros(n_slack, 1)];  % Initial guess for the decision variable (joint velocities and slack)

else
    % Define the objective function without slack variables
    %objective = @(q_vel) configurableObjective(q_vel, jacobi, xd_eff_vel, starting_joint_vel,joint_values, config);

    % % Set bounds without slack variables
    if ~config.applyVelocityDamper
        lb = max(config.gamma .* (jointminvalues - joint_values), joint_min_vel); % Element-wise max
        ub = min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel); % Element-wise min
    end
    if config.applyVelocityDamper
        lb = joint_min_vel;
        ub = joint_max_vel;
    end
    % Initial guess without slack variables
    q_vel_initial_guess = starting_joint_vel;

end

%'Display','off', ...
%'Display', 'iter', ... % Displays each iteration progress
% Set up optimization options
options = optimoptions('fmincon', 'Algorithm', 'sqp', ...
    'Display','off', ...
    'OptimalityTolerance', 1e-6, ...
    'ConstraintTolerance', 1e-6, ...
    'StepTolerance', 1e-6, ...
    'MaxIterations', 1000);

% Run the optimization
[q_vel_opt, ~, Exit_Flag] = fmincon(@(q_vel)objFun(q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix, config), q_vel_initial_guess, A, b, Aeq, beq, lb, ub, [], options);

% Separate the optimal joint velocities and slack variables if slack was applied
if config.applySlack
    optimal_joint_velocity = q_vel_opt(1:n_joints);
    %optimal_slack = q_vel_opt(n_joints+1:end);
else
    optimal_joint_velocity = q_vel_opt;
end

    starting_joint_vel = optimal_joint_velocity;
end

% Output the results
% disp('Optimal joint velocity:');
% disp(q_vel_opt);
% disp('Objective function value at optimal joint velocity:');
% disp(fval);



% if Exit_Flag <= 0
%     disp('exit Flag: ')
%     disp(Exit_Flag);
%     % Output the results
%     disp('Optimal joint velocity at infeasable solution:');
%     disp(q_vel_opt);
%     disp('Objective function value at optimal joint velocity:');
%     disp(fval);
%     if Exit_Flag < 0
%         %error('solution converged to an infeasible point')
%     end
% end



%norm((jacobi*q_vel - xd_eff_vel),2)^2 /2
%sg = 1;
% objective = @(q_vel) omega  * (norm(q_vel, Inf)^2) / 2 + ... % infinity norm term
%                        delta * (norm(q_vel, 2)^2 )/ 2 + ... % two-norm term
%                        alpha * sum((q_vel - starting_joint_vel).^2)+ ...
%                        ... %delta * norm((jacobi*q_vel - xd_eff_vel),2)^2%/2;...
%                        beta * 1/(1 + smin(jacobi)) ; % additional term from Eq. (2.17)

% sum((q_vel - current_joint_vel).^2)
% objective = @(q_vel) (1-delta-alpha-beta)  * (norm(q_vel, Inf)^2) / 2 + ... % infinity norm term
%                        delta * (norm(q_vel, 2)^2 )/ 2 + ... % two-norm term
%                        alpha * ((q_vel * eye(length(q_vel))*q_vel') + 2* sg * q_vel  ) / 2 + ...
%                        beta * 1/(1 + sqrt(det(jacobi' * jacobi))) ; % additional term from Eq. (2.17)


%options = optimoptions('fmincon', 'Algorithm', 'interior-point', ... % Choose an algorithm
% 'Display', 'iter', ... % Display iterations
% 'MaxIterations', 1000, ... % Maximum number of iterations
% 'ConstraintTolerance', 1e-6); % Constraint tolerance
%options = optimoptions('fmincon','Algorithm','sqp');
