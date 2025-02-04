close all;
joint_vector_visualisation = readmatrix('/home/shobhit/TUM_SJ_ObstacleAvoidanceLib/outputFileForVisualization.csv');
trajTimes = 0:0.001:18;

number_of_joints = 7;
joint_vector_visualization = zeros(numel(trajTimes),9);
joint_vector_visualization(:,1:number_of_joints) = joint_vector_visualisation';
% optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,obstacles);
optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,[]);

