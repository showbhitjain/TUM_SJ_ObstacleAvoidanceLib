function dXdt = joint_dynamics_integrate(t, X, joint_velocity)
    dXdt = joint_velocity; % Since velocity is constant over small intervals
end
