% Define the obstacle structure with all possible fields
%{
obstacle = struct(...
    'type', '', ...               % An integer or enum to represent the type
    'center', [0, 0, 0], ...
    'dimensions', [0, 0, 0], ... % Use for both radius-height of cylinder and dimensions of box
    'orientation', [1, 0, 0, 0], ... % Default orientation (no rotation)
    'axis', [0, 0, 1] ...         % Default axis for cylinders
);
%}
function obstacle = createObstacle(type, center, dimensions, orientation, axis)
    % Set the type field based on the input
    obstacle.type = type;

    % Set the center field
    obstacle.center = center;

    % Depending on the type, set the relevant dimensions and orientation/axis
    switch type
        case 1
            obstacle.dimensions = dimensions(1); % Use the first element as the radius
        case 2
            obstacle.dimensions = dimensions; % Use dimensions for both radius and height
            obstacle.axis = axis;
        case 3
            obstacle.dimensions = dimensions; % Use dimensions for the size of the box
            obstacle.orientation = orientation;
        otherwise
            % Handle invalid type, e.g., by setting the type to an error value or returning a flag
            obstacle.type = -1; % Indicate an error
    end
end
