function objVal = configurableObjective(q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values,WeightMatrix,config)
    objVal = 0;
    
    if config.useObjectiveNormInfinity
        objVal = objVal + config.weightNormInfinity * (norm(q_vel, Inf)^2) / 2;
    end
    
    if config.useObjectiveNormL2
        objVal = objVal + config.weightNormL2 * (q_vel' * WeightMatrix * q_vel) / 2;
    end
    
    if config.useObjectiveTrajectoryFollowing
        objVal = objVal + config.weightTrajectoryFollowing * norm((jacobi * q_vel - xd_eff_vel), 2)^2 / 2;
    end
    
    if config.useObjectiveJointAcceleration
        objVal =  objVal + config.weightJointAcceleration * ((q_vel - starting_joint_vel)' * WeightMatrix * (q_vel - starting_joint_vel)) /2 ;
    end

    if config.useObjectiveManipulability
      objVal  = objVal + config.weightManipulability * (-computeManipulabilityJacobian(joint_values') * q_vel);
    end  
    % You can add more terms similarly with their checks and weights
end
