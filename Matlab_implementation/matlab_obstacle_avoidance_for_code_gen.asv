function [Exit_Flag,joint_vector] = matlab_obstacle_avoidance_for_code_gen(obstacles)
%#codegen

%setup of robot and robot modeling using rigid body tree 




%clc; 
%clear;
%close all;

% First column defines  a
% Second column defines alpha (rad)
% Thrid column defines d 
% fourths column is theta (rad) 
%   a      alpha     d   theta
mdhparams = [
    0       0       0.333   0;
    0       -pi/2   0       0;
    0       pi/2    0.316   0;
    0.0825  pi/2    0       0;
   -0.0825 -pi/2    0.384   0;
    0       pi/2    0       0;
    0.088   pi/2    0.107   0
];

dhparams = [
    0,      -pi/2,    0.333,  0;
    0,       pi/2,    0,      0;
    0.0825,  pi/2,    0.316,  0;
   -0.0825, -pi/2,    0,      0;
    0,       pi/2,    0.384,  0;
    0.088,   pi/2,    0,      0;
    0.0003,   0,    0.107,  0];

jointMinValues = [-2.8973, -1.7628, -2.8973, -3.0718, -2.8973, 0.6500, -2.8973];
jointMaxValues = [2.8973, 1.7628, 2.8973, -0.0698, 2.8973, 3.7525, 2.8973];

jointvelMinValues = [-2.1750 -2.1750 -2.1750 -2.1750 -2.6100 -2.6100 -2.6100];
jointvelMaxValues = [2.1750 2.1750 2.1750 2.1750 2.6100 2.6100 2.6100];
Homejointpositions = [0 ,0, 0, -pi/2, 0, pi/2, pi/4];
%Homejointpositions = [0 ,0, 0, -0.08, 0, 0.7, 0];

number_of_joints = length(mdhparams);
Gripper_position = [0 0 0.103399];
Gripper_orientation = [-pi/4 0 0];
frankarobot  = loadrobot("frankaEmikaPanda");
%show(frankarobot)
body_Tcp_2 = rigidBody('Gripper_TCP');
joint_Tcp_2 = rigidBodyJoint('Gripper_TCP_frame', 'fixed');
tform_tcp_2 = trvec2tform([0 0 0.103399])*eul2tform([-pi/4 0 0]);
setFixedTransform(joint_Tcp_2,tform_tcp_2);
body_Tcp_2.Joint = joint_Tcp_2;
addBody(frankarobot,body_Tcp_2,'panda_link8');

%Set initial joint position

 Initalconfig = frankarobot.homeConfiguration;
% 
 for i = 1:7
      Initalconfig(i).JointPosition = Homejointpositions(i);
 end

% 
% % Set the modified home configuration back to the robot
% show(frankarobot, Initalconfig, ); % show function does not support code generation
%  interactiveGUI = interactiveRigidBodyTree(frankarobot);

 
%Set End effector name + set TcP at Gripper
%BUild robot from DH Parameters  [a alpha d theta] MOdiefied DH
%Create a rigid body tree. This tree is initialized with a base coordinate frame to attach bodies to.
frankarobot_mdh = rigidBodyTree(MaxNumBodies= 70,DataFormat="row");
body1 =rigidBody('robot_link1');
jnt1 = rigidBodyJoint('robot_joint1','revolute');
jnt1.HomePosition = Homejointpositions(1);
jnt1.PositionLimits = [jointMinValues(1), jointMaxValues(1)];
setFixedTransform(jnt1,mdhparams(1,:),'mdh');
body1.Joint = jnt1;

%Add the first body to the tree. 
% Specify that you are attaching it to the base of the tree. 
% The fixed transform defined previously is from the base (parent) to the first body.
addBody(frankarobot_mdh,body1,'base')


% Dynamically create and add the rest of the bodies
for i = 2:number_of_joints
    bodyName = sprintf('robot_link%d', int32(i));
    jointName = sprintf('robot_joint%d', int32(i));
    body = rigidBody(bodyName);
    joint = rigidBodyJoint(jointName, 'revolute');
    joint.HomePosition = Homejointpositions(i);
    joint.PositionLimits = [jointMinValues(i), jointMaxValues(i)];
    setFixedTransform(joint, mdhparams(i,:), 'mdh');
    body.Joint = joint;
    addBody(frankarobot_mdh, body, sprintf('robot_link%d', int32(i-1)));
end

body_Tcp = rigidBody('Gripper_TCP');
joint_Tcp = rigidBodyJoint('Gripper_TCP_frame', 'fixed');
tform_tcp = trvec2tform(Gripper_position)*eul2tform(Gripper_orientation);
setFixedTransform(joint_Tcp,tform_tcp);
body_Tcp.Joint = joint_Tcp;
addBody(frankarobot_mdh,body_Tcp,'robot_link7');

%getTransform(frankarobot,Initalconfig,'panda_link8','panda_link0');
Initalconfig_mdh = frankarobot_mdh.homeConfiguration;
robot = frankarobot_mdh;
%Import data about robot, trajectory waypoints,Initial joint confoguration etc.


clear obstacle_avoidance_equation;

%Setting up of Trajectory Waypoints
Starting_tcp_tform = getTransform(robot,robot.homeConfiguration,'Gripper_TCP','base');
toolPositionHome = Starting_tcp_tform(1:3,4);

% Define waypoints
waypoints = [toolPositionHome'; 
             toolPositionHome' + [0, 0.3 , 0.1]; 
             toolPositionHome' + [0, 0.3, -0.2]; 
             toolPositionHome' + [0, -0.3, -0.2]; 
             toolPositionHome' + [-0.3, -0.3, -0.2]];

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



%radius for each link
radius_of_links = [0.1 0.1 0.1 0.1 0.1 0.1 0.1];
d_influence = 0.04;
d_stop = 0.01;
d_buffer = 0.005;


%Configuration Paramters Setup for obstacle avoidance and Inverse Kinematics
% Example of a configuration structure for objective functions
config = struct();
config.useObjective1 = false;  % Use infinity norm term
config.weight1 = 0.05;         % Weight for objective 1

config.useObjective2 = true;  % Use two-norm term
config.weight2 = 0.3;         % Weight for objective 2

config.useObjective3 = false; % Use norm of (jacobi*q_vel - xd_eff_vel)
config.weight3 = 2;         % Weight for objective 3

config.useObjective4 = false;  % Use sum of (q_vel - q_vel_previous)^2
config.weight4 = 2;         % Weight for objective 4

config.useObjective5 = false;  % Use  1/(1 + smin(jacobi))
config.weight5 = 0.4;         % Weight for objective 5

config.useObjective6 = true; %use of manipulability constraint for Jm' * q_velocity
config.weight6 = 1;         % singularity avoidance and manipulability maximization

% Constraint configuration
config.applyEqualityConstraints = true;    % Flag to apply equality constraints
config.applyInequalityConstraints = true;  % Flag to apply inequality constraints

%Slack 
config.applySlack = true;
config.Slacklowerbound = [-0.25,-0.25,-0.25,-deg2rad(2),-deg2rad(2),-deg2rad(60)];
config.Slackupperbound = [0.25,0.25,0.25,deg2rad(2),deg2rad(2),deg2rad(60)];
config.Slack_penalty_weightmatrix = diag([1 1 1 1 1 0.5]);
config.Slack_objective_weight = 20;
%Choose either Obstacle avoidance scheme 1 or 2 otherwise it would result in error;

%Obstacle avoidance scheme1
config.obstacle_avoidance_scheme1 = false; %Flag to apply obstacle avoidance scheme_1

%Obstacle avoidance scheme1
config.obstacle_avoidance_scheme2 = true; %Flag to apply obstacle avoidance scheme_2
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





%From Here onwards the process starts 
%Trajectory Generation using trajectory points 
%COmplete Trajectory Generation
%Complete Trajectory Generation from start till end
% Cartesian Motion only
trajType = 'cubic';
switch trajType
    case 'trap'
        [xd,xd_vel,xdd] = trapveltraj(waypoints,numel(trajTimes), ...
            'AccelTime',repmat(waypointAccelTimes,[3 1]), ... 
            'EndTime',repmat(diff(waypointTimes),[3 1]));
                            
    case 'cubic'
        [xd,xd_vel,xdd] = cubicpolytraj(waypoints,waypointTimes,trajTimes, ... 
            'VelocityBoundaryCondition',waypointVels);
        
    case 'quintic'
        [xd,xd_vel,xdd] = quinticpolytraj(waypoints,waypointTimes,trajTimes, ... 
            'VelocityBoundaryCondition',waypointVels, ...
            'AccelerationBoundaryCondition',waypointAccels);
        
    case 'bspline'
        ctrlpoints = waypoints; % Can adapt this as needed
        [xd,xd_vel,xdd] = bsplinepolytraj(ctrlpoints,waypointTimes([1 end]),trajTimes);
        
    otherwise
        error('Invalid trajectory type! Use ''trap'', ''cubic'', ''quintic'', or ''bspline''');
end

desired_quaternions = zeros(4,numel(trajTimes));
desired_angular_velocity = zeros(3,numel(trajTimes));
desired_angular_accel = zeros(3,numel(trajTimes));
numWaypoints = size(orientations,2);
for w = 1:numWaypoints-1
    % Get the initial and final rotations and times for the segment
    R0 = eul2quat(orientations(:,w)');
    Rf = eul2quat(orientations(:,w+1)');
    timeInterval = waypointTimes(w:w+1);
    trajTimes_rot = timeInterval(1):ts:timeInterval(2);
    num_samples = (timeInterval(2)-timeInterval(1))/ts;
    % Generate time scaling trajectory for the segment on the range [0 1]
    switch trajType
        case 'trap'
            [s,sd,sdd] = trapveltraj([0 1],numel(trajTimes_rot), ... 
                                     'EndTime',diff(timeInterval));
        case 'cubic'
            [s,sd,sdd] = cubicpolytraj([0 1],timeInterval,trajTimes_rot);
        case 'quintic'
            [s,sd,sdd] = quinticpolytraj([0 1],timeInterval,trajTimes_rot);
        otherwise
            error('Invalid trajectory type! Use ''trap'', ''cubic'', or ''quintic''');
    end
    
    % Find the quaternions from trajectory generation
    [R, omega, alpha] = rottraj(R0, Rf, timeInterval, trajTimes_rot,'TimeScaling',[s;sd;sdd]);
    start_idx = 1+(timeInterval(1)/ts);
    end_idx = 1+(timeInterval(2)/ts);
    desired_quaternions(:,start_idx:end_idx) = R;
    desired_angular_velocity(:,start_idx:end_idx) = omega;
    desired_angular_accel(:,start_idx:end_idx) = alpha;
end



%Optimal Inverse kinematics 

%Optimal Inverse kinematics 
%config from robot 

%integrate_velocity(            )
%robot joint vector and velocity whose elements will be measured by sensor
%later on 

joint_velocity = zeros(number_of_joints, numel(trajTimes));
joint_vector = zeros(number_of_joints, numel(trajTimes));


desired_joint_velocity = zeros(number_of_joints, numel(trajTimes));

desired_joint_vector = zeros(number_of_joints, numel(trajTimes));

joint_vector(:,1) = Homejointpositions';
desired_joint_vector(:,1)  = Homejointpositions';
Kp = diag([1 1 1]) ;
Ko = diag([1 1 1]) ;

%clear integrate_velocity
%joint_velocity for obsacle avoidance at t = 0
joint_velocity_obstacle_avoidance = zeros(number_of_joints,1); 

% Open the log file before the loop starts
%logFile = fopen('obstacle_avoidance_log.txt', 'a');
mindistance = zeros(1,numel(trajTimes)-1);
translational_error = zeros(3,numel(trajTimes)-1);
rotational_error = zeros(3,numel(trajTimes)-1);

for i=1:numel(trajTimes)-1
    %Integrator with start condtion
   
   T_endeffector2base = getTransform(robot,joint_vector(:,i)','Gripper_TCP','base');
   x_current = T_endeffector2base(1:3,4);
    
   xd_effective = xd_vel(:,i) + Kp*(xd(:,i) -  x_current );
    translational_error(:,i) = xd(:,i) -  x_current;
    
   Jacobi_matrix = rearrangejacobi(geometricJacobian(robot,joint_vector(:,i)','Gripper_TCP'),number_of_joints);
   %Jacobi_matrix = jacobian_cartesian(robot,joint_vector(:,i)',8);
   
   angular_velocity_effective = desired_angular_velocity(:,i) + Ko * compute_orientation_Error(T_endeffector2base,desired_quaternions(:,i)');
   rotational_error(:,i)  =  compute_orientation_Error(T_endeffector2base,desired_quaternions(:,i)');
   %Concatenate 
   pose_velocity_effective = [xd_effective; angular_velocity_effective]; 
   
   
   
   %Obstacle_avoidance 
   if config.obstacle_avoidance_scheme1 || config.obstacle_avoidance_scheme2

       [J_g, b_g, mindistance(i)]= obstacle_avoidance_equation(robot,obstacles,joint_vector(:,i)',joint_velocity_obstacle_avoidance',mdhparams,radius_of_links,d_influence,d_stop,config,[]);
   
   else
       J_g = [];
       b_g = [];


   end
   

   %config.Slack_objective_weight = 1/ norm(final_position_TCP- x_current) ;

   [desired_joint_velocity(:,i), Exit_Flag] = inverse_kinematics_constraints(joint_vector(:,i),Jacobi_matrix,pose_velocity_effective,jointMinValues,jointMaxValues,jointvelMinValues,jointvelMaxValues,J_g,b_g,config);
   if Exit_Flag < 0
       break;
   end
   joint_velocity_obstacle_avoidance =  desired_joint_velocity(:,i);
   tspan = [trajTimes(i) trajTimes(i+1)];

    desired_joint_velocity_current = desired_joint_velocity(:, i);  % Current desired joint velocities
   [T,Y] = ode45(@(t,X)joint_dynamics_integrate(t,X,desired_joint_velocity_current),tspan,desired_joint_vector(:,i));
      % The new joint positions are the last row of Y
    desired_joint_vector(:, i+1) = Y(end, :)';
    
   iscrossed =  checkJointLimits(desired_joint_vector(:, i+1)', jointMinValues, jointMaxValues);
   if iscrossed 
         %disp('joint_limits_crossed')   
       break;
   end
   %desired_joint_vector(:,i+1) =integrate_velocity(desired_joint_velocity(:,i),ts,joint_vector(:,1)) ;
   %Add noise to simulate the measurement from robot 
   joint_vector(:,i+1) = add_noise(desired_joint_vector(:,i+1),-0.0005,0.0005);
    
%Obstacle_avoidance
%obstacle_avoidance_equation()

 %Optimal Inverse Kinematics

 %q_vel
    
end 


% Close the log file after the loop
%fclose(logFile);


% %Visualization of Results 
% joint_vector_visualization = zeros(numel(trajTimes),9);
% joint_vector_visualization(:,1:number_of_joints) = joint_vector';
% optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,obstacles);

joint_vector = joint_vector';


end