clear; close all;

import_robot_panda; 
clear obstacle_avoidance_equation;

%Setting up of Trajectory Waypoints
Starting_tcp_tform = getTransform(robot,robot.homeConfiguration,'Gripper_TCP','base');
toolPositionHome = Starting_tcp_tform(1:3,4);

% Define waypoints

% waypoints = [toolPositionHome + [0; 0; 0], ...
% toolPositionHome + [0; 0.3; 0.1], ...
%  toolPositionHome + [0; 0.3; -0.2], ...
% toolPositionHome + [0; -0.3; -0.2], ...
% toolPositionHome + [-0.3; -0.3; -0.2]];
% 
waypoints = [toolPositionHome'; 
             toolPositionHome' + [0, 0.2 , 0]; 
             toolPositionHome' + [0, 0.2, -0.2]; 
             toolPositionHome' + [0, -0.3, -0.2]; 
             toolPositionHome' + [-0.3 -0.3 -0.2]]';


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

  % [ 0,        0.3827,    0.7071,    0.3827,     0],
  %   [1.0000,    0.9239,    0.7071,    0.9239,    1.0000],
  %   [ 0,         0,         0,         0,         0],
  %   [ 0,         0,         0,         0,         0]

% orientations = [0     0    pi;
%                 0  0    3*pi/4; 
%                 0   0  pi/2;
%                 0   0    3*pi/4;
%                 0   0    pi]';   

orientations = [0.0000    1.0000         0         0;
                0.0000    1.0000         0         0;
                0.0000    1.0000         0         0;
                0.0000    1.0000         0         0;
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
[xd, xd_vel, xdd] = positionTrajectory(waypoints, waypointTimes, ts, trajType, waypointVels, waypointAccels);

[desired_quaternions,desired_angular_velocity,desired_angular_accel] = orientationTrajectory(orientations,waypointTimes,ts,trajType);

config = struct();
config.useObjectiveNormInfinity = false;  % Use infinity norm term
config.weightNormInfinity = 0.1;         % Weight for objective 1

config.useObjectiveNormL2 = true;  % Use two-norm term
config.weightNormL2 = 1;         % Weight for objective 2

config.useObjectiveTrajectoryFollowing = false; % Use norm of (jacobi*q_vel - xd_eff_vel)
config.weightTrajectoryFollowing = 2;         % Weight for objective 3

config.useObjectiveJointAcceleration = false;  % Use sum of (q_vel - q_vel_previous)^2
config.weightJointAcceleration = 2;         % Weight for objective 4

config.useObjectiveManipulability = true ; %use of manipulability constraint for Jm' * q_velocity
config.weightManipulability = 0.1;         % singularity avoidance and manipulability maximization

% Constraint configuration
config.applyEqualityConstraints = true;    % Flag to apply equality constraints
config.applyInequalityConstraints = true;  % Flag to apply inequality constraints

%Slack 
config.applySlack = true;
config.Slacklowerbound = [-0.25,-0.25,-0.25,-deg2rad(2),-deg2rad(2),-deg2rad(60)]';
config.Slackupperbound = [0.25,0.25,0.25,deg2rad(2),deg2rad(2),deg2rad(60)]';
config.SlackPenaltyWeight = [1 1 1 1 1 0.5];
config.SlackObjectiveWeight = 20;
%Choose either Obstacle avoidance scheme 1 or 2 otherwise it would result in error;


%Obstacle avoidance scheme
config.obstacleAvoidanceScheme = true; %Flag to apply obstacle avoidance scheme
config.k = 2 ; %The efficiency of the manipulator to change the velocity of
% the critical point C when avoiding obstacles increases with the for
% obstacle avoidance scheme 2 
% decrease of k, but the burden of the manipulator will increase
% at the same time.

config.gamma = 2; %gamma defines the joint limit avoidance gain Increasing gamma: 
%This means the joint can move faster since the velocity limits are less restrictive.
% However, it also means that the joint is more prone to reach its physical limits faster, 
% potentially leading to more abrupt stops or higher dynamic stresses.

%{
Decreasing gamma: Conversely, decreasing χ reduces the product gamma*(θ+  − θ) and gamma*(θ−  − θ), 
which narrows the range between the upper and lower velocity bounds. 
This constrains the joint to slower velocities, potentially increasing precision and safety.
However, it may also limit the responsiveness or speed of the system's movements.
%}

config.dynamicSlack = true;
config.applyVelocityDamper = true;

config.jointLimitActivationDistance = 10; % in degrees
config.jointLimitStopDistance = 3;
config.jointLimitGain = 1;

joint_velocity = zeros(number_of_joints, numel(trajTimes));
joint_vector = zeros(number_of_joints, numel(trajTimes));


desired_joint_velocity = zeros(number_of_joints, numel(trajTimes));

desired_joint_vector = zeros(number_of_joints, numel(trajTimes));

joint_vector(:,1) = Homejointpositions';
desired_joint_vector(:,1)  = Homejointpositions';
Kp = diag([1 1 1]);
Ko = diag([1 1 1]);

% Use an enum or integer to represent the obstacle type for efficiency
OBSTACLE_SPHERE = 1;
OBSTACLE_CYLINDER = 2;
OBSTACLE_BOX = 3;


obstacle_sphere_1 = struct(...
    'type', OBSTACLE_SPHERE, ...               
    'center', toolPositionHome' + [0, 0.2 , 0], ...
    'dimensions', [0.04, 0.04, 0.04], ... 
    'orientation', [1, 0, 0, 0], ... 
    'axis', 0 ...        
);
%center_1 [0.30, 0.20, 0.8]
obstacle_sphere_2 = struct(...
    'type', OBSTACLE_SPHERE, ...               
    'center', [0.5545, 0.20, 0.3211], ...
    'dimensions', [0.05, 0.05, 0.05], ... 
    'orientation', NaN, ... 
    'axis', NaN ...        
);
% obstacle_sphere_3 = struct('type', OBSTACLE_SPHERE, 'center', ...
%     [0.4, 0.15, 0.7], 'dimensions', [0.03, 0.03, 0.03], 'orientation', NaN, 'axis', NaN);
obstacles = [obstacle_sphere_1 obstacle_sphere_2];
%obstacle_bus  =Simulink.Bus.createObject(obstacles);

%radius for each link
radius_of_links = [0.1 0.1 0.1 0.1 0.1 0.1 0.1];
d_influence = 0.04;
d_stop = 0.01;


clear integrate_velocity
%joint_velocity for obsacle avoidance at t = 0
joint_velocity_obstacle_avoidance = zeros(number_of_joints,1); 

% Open the log file before the loop starts
%logFile = fopen('obstacle_avoidance_log.txt', 'a');
mindistance = zeros(1,numel(trajTimes)-1) ;
translational_error = zeros(3,numel(trajTimes)-1) ;
rotational_error = zeros(3,numel(trajTimes)-1) ;

jointMinValues = jointMinValues' ;
jointMaxValues = jointMaxValues' ;
jointvelMinValues = jointvelMinValues' ; 
jointvelMaxValues = jointvelMaxValues' ; 


sphere_Center = [0.5545,0.20,0.5211];
sphere_Radius = 0.08;
linesegments = createLineSegments(mdhparams,robot,radius_of_links,Homejointpositions);
[s,dist,closestPointLink,closesePointObstacle]  = calculate_distance_lss_pss(linesegments(7).aSegmentV0,linesegments(7).aSegmentV1,radius_of_links(1),sphere_Center,sphere_Radius);

 transform = getTransform(robot,Homejointpositions,'base','robot_link6');
closestPointLinkNew = [(closestPointLink)';1];
relativeLinkNewHom = transform * closestPointLinkNew ;
relativeLinkPoint = relativeLinkNewHom(1:3)';
addCriticalPoint(robot,'relativeBody','relativeJOINT',relativeLinkPoint,7);

transformRelative = getTransform(robot,Homejointpositions,'relativeBody','base');
 JacobiRelative = rearrangejacobi(geometricJacobian(robot,Homejointpositions,'relativeBody'),7);

for i=1:numel(trajTimes)-1
    %Integrator with start condtion
   
   T_endeffector2base = getTransform(robot,joint_vector(:,i)','Gripper_TCP','base') ;
   x_current = T_endeffector2base(1:3,4) ;
    
   xd_effective = xd_vel(:,i) + Kp*(xd(:,i) -  x_current ) ;
   translational_error(:,i) = xd(:,i) -  x_current ;
    
   Jacobi_matrix = rearrangejacobi(geometricJacobian(robot,joint_vector(:,i)','Gripper_TCP'),number_of_joints);
   %Jacobi_matrix = jacobian_cartesian(robot,joint_vector(:,i)',8);
   
   angular_velocity_effective = desired_angular_velocity(:,i) + Ko* compute_orientation_Error(T_endeffector2base,desired_quaternions(:,i)');
   rotational_error(:,i)  =  compute_orientation_Error(T_endeffector2base,desired_quaternions(:,i)');
   %Concatenate 
   pose_velocity_effective = [xd_effective; angular_velocity_effective]; 
    
   %Obstacle_avoidance 
   if  config.obstacleAvoidanceScheme
       [J_g, b_g, mindistance(i)]= obstacle_avoidance_equation(robot,obstacles,joint_vector(:,i)',joint_velocity_obstacle_avoidance',mdhparams,radius_of_links,d_influence,d_stop,config,[]);
   
   else
       J_g = [];
       b_g = [];
   end 
   
       jointVelocityWeightMatrix = diag(ones(1,number_of_joints));
 
  
   %     J_g
   %  b_g
   % 
   % if(i == 4000)
   %     break;
   % end
   [desired_joint_velocity(:,i), Exit_Flag] = inverseKinematicsOAModified(joint_vector(:,i),Jacobi_matrix,pose_velocity_effective,jointMinValues,jointMaxValues,jointvelMinValues,jointvelMaxValues,J_g,b_g,jointVelocityWeightMatrix,config);
   
   
   if Exit_Flag < 0
       disp(Exit_Flag);
       break;
   end

   
   tspan = [trajTimes(i) trajTimes(i+1)];
    
   desired_joint_velocity_current = desired_joint_velocity(:, i);  % Current desired joint velocities
   joint_velocity_obstacle_avoidance = desired_joint_velocity_current ;
   [T,Y] = ode45(@(t,X)joint_dynamics_integrate(t,X,desired_joint_velocity_current),tspan,desired_joint_vector(:,i));
      % The new joint positions are the last row of Y
   desired_joint_vector(:, i+1) = Y(end, :)';
    
   % iscrossed =  checkJointLimits(desired_joint_vector(:, i+1)', jointMinValues, jointMaxValues);
   % if iscrossed 
   %       %disp('joint_limits_crossed')   
   %     break;
   %  end
   %desired_joint_vector(:,i+1) =integrate_velocity(desired_joint_velocity(:,i),ts,joint_vector(:,1)) ;
   %Add noise to simulate the measurement from robot 
   joint_vector(:,i+1) = add_noise(desired_joint_vector(:,i+1),-0.0005,0.0005);
    
    
end 



% Close the log file after the loop
%fclose(logFile);


%Visualization of Results 
joint_vector_visualization = zeros(numel(trajTimes),9);
joint_vector_visualization(:,1:number_of_joints) = joint_vector';
optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,obstacles);







