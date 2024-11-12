function [xd, xd_vel, xdd] = cartesian_trajectory(waypoints, waypointTimes, ts, trajType, waypointVels, waypointAccels)
%#codegen

%trajectory interpolation between waypoints
% INPUTS:
% waypoints: size = (3xn), row order x;y;z
% waypointTimes : timepoints at waypoints
% ts : sample time
% trajType : type of interpolation trajectory 'trap' , 'cubic' or 'quintic' possible values 
% waypointVels: optional input (only for 'cubic' or 'quintic') with default value to be chosen as same in the code 
% waypointAccels: optional input (only for 'quintic' ) with default value to be chosen as same in the code 

%OUTPUTS: [xd,xd_vel,xdd]
%xd: desired cartesian order x;y;z (3xn) positions between waypoints
% xd_vel: desired cartesian velocity (3xn)
% xdd : desired cartesian accelration (3xn)
% Validate input arguments
    if nargin < 5 || isempty(waypointVels)
        waypointVels = 0.1 * zeros(3, size(waypoints, 2)); % Default velocities
    end
    if nargin < 6 || isempty(waypointAccels)
        waypointAccels = zeros(size(waypointVels)); % Default accelerations
    end

    % Calculate acceleration times if applicable
    waypointAccelTimes = 0.25 * diff(waypointTimes);

    % Define trajectory sample times
    trajTimes = 0:ts:waypointTimes(end);
    coder.varsize("xd",[3 Inf],[0 1]);
    coder.varsize("xd_vel",[3 Inf],[0 1]);
    coder.varsize("xdd",[3 Inf],[0 1]);
    
    
    % Switch between trajectory types and generate the desired trajectory
    switch trajType
        case 'trap'
            [xd, xd_vel, xdd] = trapveltraj(waypoints, numel(trajTimes), ...
                'AccelTime', repmat(waypointAccelTimes, [3, 1]), ...
                'EndTime', repmat(diff(waypointTimes), [3, 1]));
        case 'cubic'
            [xd, xd_vel, xdd] = cubicpolytraj(waypoints, waypointTimes, trajTimes, ...
                'VelocityBoundaryCondition', waypointVels);
        case 'quintic'
            [xd, xd_vel, xdd] = quinticpolytraj(waypoints, waypointTimes, trajTimes, ...
                'VelocityBoundaryCondition', waypointVels, ...
                'AccelerationBoundaryCondition', waypointAccels);
        otherwise
            error('Invalid trajectory type! Use ''trap'', ''cubic'', ''quintic'', or ''bspline''');
    end
end