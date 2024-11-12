clear;

import_robot_panda; 
clear obstacle_avoidance_equation;

%Setting up of Trajectory Waypoints
Starting_tcp_tform = getTransform(robot,robot.homeConfiguration,'Gripper_TCP','base');
toolPositionHome = Starting_tcp_tform(1:3,4);

% Define waypoints

waypoints = [toolPositionHome + [0; 0; 0], ...
toolPositionHome + [0; 0.3; 0.1], ...
 toolPositionHome + [0; 0.3; -0.2], ...
toolPositionHome + [0; -0.3; -0.2], ...
toolPositionHome + [-0.3; -0.3; -0.2]];
% 
% waypoints = [toolPositionHome'; 
%              toolPositionHome' + [0, 0.2 , 0]; 
%              toolPositionHome' + [0, 0.2, -0.2]; 
%              toolPositionHome' + [0, -0.3, -0.2]; 
%              toolPositionHome' + [-0.3 -0.3 -0.2]]';


%  toolPositionHome' + [0, 0.3 , 0.1]; second waypoint
% waypoints = [toolPositionHome'; 
%              toolPositionHome' + [0, 0 , 0]; 
%              toolPositionHome' + [0, 0, 0]; 
%              toolPositionHome' + [0, -0, 0]; 
%              toolPositionHome']';
% + [0, 0, 0.2]
% Ensure waypoints are transposed correctly for vertical concatenation

         
final_position_TCP = waypoints(:,end);
% Euler Angles (Z Y X) relative to the home orientation       
% orientations = [0     0    pi;
%                 0  0    3*pi/4; 
%                 0   0  pi/2;
%                 0   0    3*pi/4;
%                 0   0    pi]';   

orientations = [0.0000    1.0000         0         0;
    0.3827    0.9239         0         0;
    0.7071    0.7071         0         0;
    0.3827    0.9239         0         0;
    0.0000    1.0000         0         0]';
     % % 
     % orientations = [0     0    pi;
     %                0  0    pi; 
     %                0   0  pi;
     %                0   0    pi;
     %                0   0    pi]';          

     
 
     % Array of waypoint times
waypointTimes = 0:4:16;
%waypointTimes = 0:3:12;
% Trajectory sample time
ts = 0.001; %1 ms


%% Additional parameters

% Boundary conditions (for polynomial trajectories)
% Velocity (cubic and quintic)
% waypointVels = 0.1 *[ 0  1  0;
%                      -1  0  0;
%                       0 -1  0;
%                       1  0  0;
%                       0  1  0]';
waypointVels = 0.1 *[ 0  0  0;
                     0  0  0;
                      0 0  0;
                      0  0  0;
                      0  0  0]';
% Acceleration (quintic only)
waypointAccels = zeros(size(waypointVels));

% Acceleration times (trapezoidal only)
waypointAccelTimes = 0.25 * diff(waypointTimes);
trajType = 'cubic'; % Trajectory type
trajTimes = 0:ts:waypointTimes(end);


%From Here onwards the process starts 
%Trajectory Generation using trajectory waypoints 
% Cartesian Motion only
% Call the trajectory generation function
[xd, xd_vel, xdd] = cartesian_trajectory(waypoints, waypointTimes, ts, trajType, waypointVels, waypointAccels);

[desired_quaternions,desired_angular_velocity,desired_angular_accel] = orientation_trajectory(orientations,waypointTimes,ts,trajType);



