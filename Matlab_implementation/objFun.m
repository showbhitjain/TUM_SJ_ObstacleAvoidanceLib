function objVal = objFun(q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values, config)
    n_joints = length(joint_values);
    objVal = configurableObjective(q_vel(1:n_joints), jacobi, xd_eff_vel, starting_joint_vel, joint_values, config);
    
    % Add the cost for the slack variables
    if config.applySlack
        slack = q_vel(n_joints+1:end);
        objVal = objVal + config.Slack_objective_weight * slack' * config.Slack_penalty_weightmatrix * slack;
    end
end