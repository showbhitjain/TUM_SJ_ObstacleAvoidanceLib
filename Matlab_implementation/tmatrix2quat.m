function quat = tmatrix2quat(T)

% Make sure the rotation matrix is 3x3
R = T(1:3, 1:3);

% Preallocate the quaternion
quat = zeros(1, 4);

% Calculate the trace of the matrix
tr = trace(R);

% Check the value of the trace
if tr > 0
    S = sqrt(tr + 1.0) * 2; % S=4*qw
    quat(1) = 0.25 * S;
    quat(2) = (R(3,2) - R(2,3)) / S;
    quat(3) = (R(1,3) - R(3,1)) / S;
    quat(4) = (R(2,1) - R(1,2)) / S;
elseif (R(1,1) > R(2,2)) && (R(1,1) > R(3,3))
    S = sqrt(1.0 + R(1,1) - R(2,2) - R(3,3)) * 2; % S=4*qx
    quat(1) = (R(3,2) - R(2,3)) / S;
    quat(2) = 0.25 * S;
    quat(3) = (R(1,2) + R(2,1)) / S;
    quat(4) = (R(1,3) + R(3,1)) / S;
elseif R(2,2) > R(3,3)
    S = sqrt(1.0 + R(2,2) - R(1,1) - R(3,3)) * 2; % S=4*qy
    quat(1) = (R(1,3) - R(3,1)) / S;
    quat(2) = (R(1,2) + R(2,1)) / S;
    quat(3) = 0.25 * S;
    quat(4) = (R(2,3) + R(3,2)) / S;
else
    S = sqrt(1.0 + R(3,3) - R(1,1) - R(2,2)) * 2; % S=4*qz
    quat(1) = (R(2,1) - R(1,2)) / S;
    quat(2) = (R(1,3) + R(3,1)) / S;
    quat(3) = (R(2,3) + R(3,2)) / S;
    quat(4) = 0.25 * S;
end

% Normalize the quaternion
quat = quat / norm(quat);
end