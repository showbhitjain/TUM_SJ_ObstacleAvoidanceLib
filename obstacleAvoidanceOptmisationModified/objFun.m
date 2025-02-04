function objVal = objFun(q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix, config,slackIsUsed)
    n_joints = length(joint_values);
    objVal = configurableObjective(q_vel(1:n_joints), jacobi, xd_eff_vel, starting_joint_vel, joint_values,jointVelocityWeightMatrix, config);
    
     % Add the cost for the slack variables
    if slackIsUsed
        slack = q_vel(n_joints+1:end);
        objVal = objVal + config.SlackObjectiveWeight * slack' * diag(config.SlackPenaltyWeight) * slack;
    end
end