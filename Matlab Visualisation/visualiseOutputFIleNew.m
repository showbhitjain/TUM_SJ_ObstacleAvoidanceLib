clear; close all;

import_robot_panda; 

% Enum-like constants
OBSTACLE_SPHERE   = 1;
OBSTACLE_CYLINDER = 2;
OBSTACLE_BOX      = 3;

% Type mapping from string to integer enum
typeMap = struct('Sphere', OBSTACLE_SPHERE, ...
                 'Cylinder', OBSTACLE_CYLINDER, ...
                 'Box', OBSTACLE_BOX);

% Read and decode the JSON
jsonText = fileread('../config/configDemo/Obstacles.json');
data = jsondecode(jsonText);

% Get all obstacle keys
obstacleNames = fieldnames(data.Obstacles);
numObstacles = length(obstacleNames);

% Preallocate struct array
obstacles(numObstacles) = struct('type', [], 'center', [], 'dimensions', [], 'orientation', [], 'axis', [], 'color', []);

% Default color map by type (optional)
defaultColors = containers.Map(...
    {OBSTACLE_SPHERE, OBSTACLE_CYLINDER, OBSTACLE_BOX}, ...
    {[1 0 0], [0 1 0], [0 0 1]});  % red, green, blue

for i = 1:numObstacles
    name = obstacleNames{i};
    o = data.Obstacles.(name);

    % Map type string to enum
    typeStr = o.type;
    if isfield(typeMap, typeStr)
        type = typeMap.(typeStr);
    else
        error("Unknown obstacle type: %s", typeStr);
    end

    % Assign required fields
    obstacles(i).type = type;
    obstacles(i).center = o.center';
    obstacles(i).dimensions = o.dimensions';

    % Orientation
    if isfield(o, 'orientation')
        obstacles(i).orientation = o.orientation';
    else
        obstacles(i).orientation = NaN;
    end

    % Axis (optional)
    if isfield(o, 'axis')
        obstacles(i).axis = o.axis';
    else
        obstacles(i).axis = NaN;
    end

    % Assign default color based on type
    if isKey(defaultColors, type)
        obstacles(i).color = defaultColors(type);
    else
        obstacles(i).color = [0.5 0.5 0.5]; % gray as fallback
    end
end

%Get the waypoints 
waypointsText = fileread('../config/configDemo/trajectoryConfig.json');
waypointsData = jsondecode(waypointsText);

waypoints = waypointsData.Waypoints;

pandaModelingText = fileread('../config/configDemo/pandaRobot.json');
pandaRobotData = jsondecode(pandaModelingText);

radiusLinks = pandaRobotData.ObstacleAvoidanceParameters.radiusLinks;
radiusJoints = pandaRobotData.ObstacleAvoidanceParameters.radiusJoints;
finalLinkDimensions = pandaRobotData.ObstacleAvoidanceParameters.finalLinkDimensions;

joint_vector_visualisation = readmatrix('../outputDemo/robotJointPositions.csv');
% trajTimes = 0:0.001:20;
% radiusLinks = [0.1,0.1,0.07,0.08,0.07,0.0,0.08];
% radiusJoints = [0.1,0.1,0.06,0.04,0.05,0.0,0.08];
% finalLinkDimensions = [0.06,0.2,0.11];
number_of_joints = 7;
joint_vector_visualization = zeros(length(joint_vector_visualisation),9);
joint_vector_visualization(:,1:number_of_joints) = joint_vector_visualisation';
% optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,obstacles);
%optimizedVisualizeRobot_2('frankarobot',joint_vector_visualization,waypoints,2,[]);
optimizedVisualizeRobot_withNewRobotModeling('frankarobot',robot,joint_vector_visualization,waypoints,2,obstacles,mdhparams,radiusLinks,radiusJoints, finalLinkDimensions  );
