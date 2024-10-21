function [optimal_joint_velocity,Exit_Flag]  = inverse_kinematics_constraints(joint_values,jacobi,xd_eff_vel,jointminvalues,jointmaxvalues,joint_min_vel,joint_max_vel,J_g,b_g,config_input)
persistent starting_joint_vel is_first_step

n_joints = length(joint_values);  % Assuming 'joint_values' is a column vector
n_slack = 6;                       % Number of slack variables, one for each end-effector velocity component
%total_variables = n_joints + n_slack;  % Total number of variables in the optimization problem

% Ensure sizes are compatible for code generation
coder.varsize('starting_joint_vel', [n_joints, 1], [true, false]);
coder.varsize('optimal_joint_velocity', [n_joints, 1], [true, false]);

  config = config_input;

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
jointminvalues = jointminvalues';
jointmaxvalues = jointmaxvalues';
joint_min_vel = joint_min_vel';
joint_max_vel = joint_max_vel';

Weightmatrix = diag([1 1 1 1 1 1]);
% delta = weights(1);
% alpha = weights(2);
% beta = weights(3);
% omega = weights(4);

% Default initializations
    Aeq = [];
    beq = [];
    A = [];
    b = [];

    % Check if equality constrjoint_valuesaints should be applied
    if config.applyEqualityConstraints
        Aeq = Weightmatrix * jacobi;
        beq = xd_eff_vel;
    end

    % Check if inequality constraints should be applied
    if config.applyInequalityConstraints
        
        A = J_g;
        b = b_g;
    
    end

    
 if config.applySlack
    % Define the augmented objective function that includes slack penalty
    % objective = @(q_vel) configurableObjective(q_vel(1:n_joints), jacobi, xd_eff_vel, starting_joint_vel, joint_values,config) ...
    %          + config.Slack_objective_weight* q_vel(n_joints+1:end)' * config.Slack_penalty_weightmatrix * q_vel(n_joints+1:end) ;

    % Augment the bounds for the slack variables
    lb = [max(config.gamma .* (jointminvalues - joint_values), joint_min_vel); config.Slacklowerbound']; % Slack lower bounds
    ub = [min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel); config.Slackupperbound']; % Slack upper bounds
    
    % Augment the equality constraint matrices for slack variables
    if config.applyEqualityConstraints
    
        Aeq = [Weightmatrix * jacobi, eye(n_slack)];
        beq = xd_eff_vel;  % Make sure this accounts for the slack if necessary

    end 

    if config.applyInequalityConstraints 
        num_rows = size(J_g,1);
        A = [J_g, zeros(num_rows,n_slack)];
        b = b_g;  % Make sure this accounts for the slack if necessary
    end 
    % Initial guess should include the slack variables
    q_vel_initial_guess = [starting_joint_vel; zeros(n_slack, 1)];  % Initial guess for the decision variable (joint velocities and slack)
    if isempty(J_g) && isempty(b_g)
        Aeq = Weightmatrix * jacobi;
        beq = xd_eff_vel;
        A = J_g;
        b = b_g;
        config.applySlack = false;
        % Define the objective function without slack variables
        %objective = @(q_vel) configurableObjective(q_vel, jacobi, xd_eff_vel, starting_joint_vel,joint_values, config);

        % Set bounds without slack variables
        lb = max(config.gamma .* (jointminvalues - joint_values), joint_min_vel); % Element-wise max
        ub = min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel); % Element-wise min

        % Initial guess without slack variables
        q_vel_initial_guess = starting_joint_vel;
    end

 else
    % Define the objective function without slack variables
    %objective = @(q_vel) configurableObjective(q_vel, jacobi, xd_eff_vel, starting_joint_vel,joint_values, config);

    % Set bounds without slack variables
    lb = max(config.gamma .* (jointminvalues - joint_values), joint_min_vel); % Element-wise max
    ub = min(config.gamma .* (jointmaxvalues - joint_values), joint_max_vel); % Element-wise min

    % Initial guess without slack variables
    q_vel_initial_guess = starting_joint_vel;
end
   
%'Display','off', ...     
%'Display', 'iter', ... % Displays each iteration progress
% Set up optimization options
options = optimoptions('fmincon', 'Algorithm', 'sqp', ...
                                            ...
                       'OptimalityTolerance', 1e-6, ...
                       'ConstraintTolerance', 1e-6, ...
                       'StepTolerance', 1e-6, ...
                       'MaxIterations', 1000);

% Run the optimization
[q_vel_opt, ~, Exit_Flag] = fmincon(@(q_vel)objFun(q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values, config), q_vel_initial_guess, A, b, Aeq, beq, lb, ub, [], options);

% Separate the optimal joint velocities and slack variables if slack was applied
if config.applySlack
    optimal_joint_velocity = q_vel_opt(1:n_joints);
    %optimal_slack = q_vel_opt(n_joints+1:end);
else
    optimal_joint_velocity = q_vel_opt;
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

starting_joint_vel = optimal_joint_velocity;


end

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
