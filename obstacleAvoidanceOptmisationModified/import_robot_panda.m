%clc; 
%clear;
%close all;

% First column defines  a
% Second column defines alpha (rad)
% Thrid column defines d 
% fourths column is theta (rad) 
% %   a      alpha     d   theta
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
 show(frankarobot, Initalconfig,'Frames','on' ); % show function does not support code generation
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
    bodyName = ['robot_link', num2str(i)];
    jointName = ['robot_joint', num2str(i)];
    body = rigidBody(bodyName);
    joint = rigidBodyJoint(jointName, 'revolute');
    joint.HomePosition = Homejointpositions(i);
    joint.PositionLimits = [jointMinValues(i), jointMaxValues(i)];
    setFixedTransform(joint, mdhparams(i,:), 'mdh');
    body.Joint = joint;
    addBody(frankarobot_mdh, body, ['robot_link', num2str(i-1)]);
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
show(robot,Initalconfig_mdh,'Frames','on');
%Import data about robot, trajectory waypoints,Initial joint confoguration etc.