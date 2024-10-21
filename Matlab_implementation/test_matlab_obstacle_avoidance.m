clear;
%Set the positions and properties of Obstacle for testing different scenrarios 
% Use an enum or integer to represent the obstacle type for efficiency
OBSTACLE_SPHERE = 1;
OBSTACLE_CYLINDER = 2;
OBSTACLE_BOX = 3;

obstacle_sphere_1 = struct(...
    'type', OBSTACLE_SPHERE, ...               
    'center', [0.5545, 0.30, 0.6211], ...
    'dimensions', [0.04, 0.04, 0.04], ... 
    'orientation', [1, 0, 0, 0], ... 
    'axis', 0 ...        
);
%center_1 [0.30, 0.20, 0.8]
obstacle_sphere_2 = struct(...
    'type', OBSTACLE_SPHERE, ...               
    'center', [0.3, -0.25, 0.6], ...
    'dimensions', [0.05, 0.05, 0.05], ... 
    'orientation', NaN, ... 
    'axis', NaN ...        
);

obstacles = [obstacle_sphere_1 obstacle_sphere_2];
%obstacle_bus  =Simulink.Bus.createObject(obstacles);
%obstacles = [];
%0 -0.25 0.8
[Exit_Flag,joint_vector] = matlab_obstacle_avoidance_for_code_gen(obstacles);

import_robot_panda;

%Setting up of Trajectory Waypoints
Starting_tcp_tform = getTransform(robot,robot.homeConfiguration,'Gripper_TCP','base');
toolPositionHome = Starting_tcp_tform(1:3,4);

% Define waypoints
waypoints = [toolPositionHome'; 
             toolPositionHome' + [0, 0.3 , 0.1]; 
             toolPositionHome' + [0, 0.3, -0.2]; 
             toolPositionHome' + [0, -0.3, -0.2]; 
             toolPositionHome' + [-0.3 -0.3 -0.2]];

% 
% waypoints = [toolPositionHome'; 
%              toolPositionHome' + [0, 0.2 , 0]; 
%              toolPositionHome' + [0, 0.2, -0.2]; 
%              toolPositionHome' + [0, -0.3, -0.2]; 
%              toolPositionHome' + [-0.3 -0.3 -0.2]];


%  toolPositionHome' + [0, 0.3 , 0.1]; second waypoint
% waypoints = [toolPositionHome'; 
%              toolPositionHome' + [0, 0 , 0]; 
%              toolPositionHome' + [0, 0, 0]; 
%              toolPositionHome' + [0, -0, 0]; 
%              toolPositionHome'];
% + [0, 0, 0.2]
% Ensure waypoints are transposed correctly for vertical concatenation
waypoints = waypoints';
         
final_position_TCP = waypoints(:,end);
% Euler Angles (Z Y X) relative to the home orientation       
% orientations = [0     0    pi;
%                 0  0    3*pi/4; 
%                 0   0  pi/2;
%                 0   0    3*pi/4;
%                 0   0    pi];   

     orientations = [0     0    pi;
                0  0    pi; 
                0   0  pi;
                0   0    pi;
                0   0    pi];          

     
    orientations = orientations' ;
     % Array of waypoint times
waypointTimes = 0:4:16;

% Trajectory sample time
ts = 0.001; %1 ms
trajTimes = 0:ts:waypointTimes(end);

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
waypointAccelTimes = diff(waypointTimes)/4;
mdhparams = [
    0       0       0.333   0;
    0       -pi/2   0       0;
    0       pi/2    0.316   0;
    0.0825  pi/2    0       0;
   -0.0825 -pi/2    0.384   0;
    0       pi/2    0       0;
    0.088   pi/2    0.107   0
];


%radius for each link
radius_of_links = [0.1 0.1 0.1 0.1 0.1 0.1 0.1];
%Visualization of Results 
joint_vector_visualization = zeros(numel(trajTimes),9);
joint_vector_visualization(:,1:number_of_joints) = joint_vector;

optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,obstacles);

%optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,obstacles);
