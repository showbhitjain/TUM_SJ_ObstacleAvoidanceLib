function [b_0] = compute_b0(r_oc,Jacobi_critical_max,joint_velocity_critical,d_stop,d_influence,k)

b_0_first = -(r_oc ./ norm(r_oc))* Jacobi_critical_max * joint_velocity_critical;


if b_0_first > 0
    b_0 = smoothing_constraint_scheme2(norm(r_oc),d_stop,d_influence,k) * b_0_first;
elseif b_0_first <= 0
    b_0 = b_0_first;
else
    b_0 = NaN;
end

end

