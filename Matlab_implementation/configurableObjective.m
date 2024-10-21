function objVal = configurableObjective(q_vel, jacobi, xd_eff_vel, starting_joint_vel, joint_values,config)
    objVal = 0;
    
    if config.useObjective1
        objVal = objVal + config.weight1 * (norm(q_vel, Inf)^2) / 2;
    end
    
    if config.useObjective2
        objVal = objVal + config.weight2 * (norm(q_vel, 2)^2) / 2;
    end
    
    if config.useObjective3
        objVal = objVal + config.weight3 * norm((jacobi * q_vel - xd_eff_vel), 2)^2 / 2;
    end
    
    if config.useObjective4
        objVal = objVal + config.weight4 * sum((q_vel - starting_joint_vel).^2);
    end
    
    if config.useObjective5
        objVal  = objVal + config.weight5 * 1/(1 + smin(jacobi));
    end

    if config.useObjective6
      objVal  = objVal + config.weight6 * (-computeManipulabilityJacobian(joint_values') * q_vel);
    end  
    % You can add more terms similarly with their checks and weights
end
