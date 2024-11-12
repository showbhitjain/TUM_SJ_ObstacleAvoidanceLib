function [desired_quaternions,desired_angular_velocity,desired_angular_accel] = orientation_trajectory(orientations,waypointTimes,ts,trajType) %#codegen

%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
  


% Define trajectory sample times
    trajTimes = 0:ts:waypointTimes(end);

desired_quaternions = zeros(4,numel(trajTimes));
desired_angular_velocity = zeros(3,numel(trajTimes));
desired_angular_accel = zeros(3,numel(trajTimes));
numWaypoints = size(orientations,2);


% Loop through trajectory generation
for w = 1:numWaypoints-1
    
    if size(orientations,1) == 3
        R0 = quaternion(eul2quat(orientations(:,w)'));
        Rf = quaternion(eul2quat(orientations(:,w+1)'));
    elseif size(orientations,1) == 4
        R0 = quaternion(orientations(:,w)')  ;
        Rf = quaternion(orientations(:,w+1)');
    else
        error('Invalid Orientation')
    end
    timeInterval = waypointTimes(w:w+1);

    % Recalculate trajectory times for the segment
    trajTimes_rot = timeInterval(1):ts:timeInterval(2);
    num_samples = round((timeInterval(2) - timeInterval(1))/ts) + 1;

    % Use appropriate trajectory generation method
    switch trajType
        case 'trap'
            [s,sd,sdd] = trapveltraj([0 1],num_samples, ... 
                                     'AccelTime',diff(timeInterval)/3, ...
                                     'EndTime',diff(timeInterval)*2/3);
        case 'cubic'
            [s,sd,sdd] = cubicpolytraj([0 1],timeInterval,trajTimes_rot);
        case 'quintic'
            [s,sd,sdd] = quinticpolytraj([0 1],timeInterval,trajTimes_rot);
        otherwise
            error('Invalid trajectory type! Use ''trap'', ''cubic'', or ''quintic''');
    end

    % Generate the rotation trajectory
    [R, omega, alpha] = rottraj(R0, Rf, timeInterval, trajTimes_rot, 'TimeScaling', [s; sd; sdd]);

    % Calculate start and end indices for the full trajectory
    start_idx = round(timeInterval(1)/ts) + 1;
    end_idx = round(timeInterval(2)/ts) + 1;

    % Insert data
    desired_quaternions(:, start_idx:end_idx) = compact(R)';
    desired_angular_velocity(:, start_idx:end_idx) = omega;
    desired_angular_accel(:, start_idx:end_idx) = alpha;
end

