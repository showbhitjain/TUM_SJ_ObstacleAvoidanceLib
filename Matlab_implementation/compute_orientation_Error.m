%T_current: Current transformation  of endeffector to base
%desired_q should be 1x4
function [angular_velocity_correction_base] = compute_orientation_Error(T_current,desired_q)

 %current_quaternion = tmatrix2quat(T_current);
    current_quaternion = tform2quat(T_current);
 orientation_error = computequatError(desired_q,current_quaternion);

% Quaternion to axis-angle
axis_angle = quat2axang(orientation_error);
% Axis-angle to angular velocity (assuming 1 unit time for simplicity)
angular_velocity_correction = axis_angle(4) .* axis_angle(1:3)';
R = T_current(1:3, 1:3);

%Transform the Angular Velocity:
angular_velocity_correction_base = R * angular_velocity_correction;


end