function joint_values = integrate_velocity(joint_velocity, delta_t, initial_joint_value)
    % joint_velocity: an array of joint velocity values at each time step
    % delta_t: the time difference between each time step
    % initial_joint_value: the initial value of the joint at the start of the simulation
    % initial_joint_velocity: the initial value of the joint_velocity at the start of the simulation
    % joint_values: the resulting array of joint positions
    
    persistent joint_values_laststep 
    
    %joint_velocity_laststep
    
    if isempty(joint_values_laststep) %&& isempty(joint_velocity_laststep)
        joint_values_laststep = initial_joint_value;
        %joint_velocity_laststep = initial_joint_velocity;
    end

    %euler
    joint_values = joint_values_laststep + joint_velocity .* delta_t;
    
    joint_values_laststep = joint_values;
    %joint_velocity_laststep = joint_velocity;
end
    
    
