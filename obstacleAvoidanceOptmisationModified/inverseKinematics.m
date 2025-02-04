function [optimal_joint_velocity,Exit_Flag]  = inverseKinematics(joint_values,jacobi,xd_eff_vel,jointminvalues,jointmaxvalues,joint_min_vel,joint_max_vel,config)

persistent starting_joint_vel is_first_step

n_joints = length(joint_values);  % Assuming 'joint_values' is a column vector

%total_variables = n_joints + n_slack;  % Total number of variables in the optimization problem
% Ensure sizes are compatible for code generation

 %coder.varsize('starting_joint_vel', [n_joints, 1], [true, false]);
% coder.varsize('optimal_joint_velocity', [n_joints, 1], [true, false]);

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

Weightmatrix = diag([1 1 1 1 1 1]);

% Default initializations
Aeq = Weightmatrix * jacobi;
beq = xd_eff_vel;
 A = [];
 b = [];
lb = joint_min_vel;
ub = joint_max_vel;
   

% Initial guess should include the slack variables
q_vel_initial_guess = starting_joint_vel;  % Initial guess for the decision variable (joint velocities and slack)


%'Display','off', ...     
%'Display', 'iter', ... % Displays each iteration progress


% Set up optimization options
options = optimoptions('fmincon', 'Algorithm', 'sqp', ...
                         'Display', 'iter',                   ...
                       'OptimalityTolerance', 1e-6, ...
                       'ConstraintTolerance', 1e-6, ...
                       'StepTolerance', 1e-6, ...
                       'MaxIterations', 1000);

% Run the optimization
[q_vel_opt, ~, Exit_Flag] = fmincon(@(q_vel)objFun(q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values, config), q_vel_initial_guess, A, b, Aeq, beq, lb, ub, [], options);

% Separate the optimal joint velocities and slack variables if slack was applied
optimal_joint_velocity = q_vel_opt;
    
starting_joint_vel = optimal_joint_velocity;

end
