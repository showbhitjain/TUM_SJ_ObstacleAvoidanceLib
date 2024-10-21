
%input v0 ,v1 should be column 1x3
function plotCylinder(v0, v1, radius,faceColor,faceopacity)

if nargin < 4
    % If faceColor is not specified, use a default color
    faceColor = [1, 0, 0]; % Default to red
    faceopacity = 0.3;
 end  
% Number of points around the circumference of the cylinder
    numPoints = 20;

    % Create the base cylinder along the local Z-axis
    [X, Y, Z] = cylinder(radius, numPoints);
    Z = Z * norm(v1 - v0);  % Scale Z by the length of the cylinder
    
    % Calculate the transformation to align the cylinder with the points v0 and v1
direction = (v1 - v0) / norm(v1 - v0); % Normalize the direction vector
up = [0; 0; 1]; % Z-axis points up

% Calculate the axis and angle for the rotation
axis = cross(up, direction);
angle = acos(dot(up, direction));

% Create the rotation matrix
if norm(axis) ~= 0
    axis = axis / norm(axis);  % Normalize the axis
    axang = [axis', angle];    % Concatenate to form the axis-angle array
    R = axang2rotm(axang);    % Create the rotation matrix
else
    % The direction is already aligned with the Z-axis
    R = eye(3);
end
    
    % Rotate and translate the cylinder points
    newPoints = R * [X(:), Y(:), Z(:)]';  % Apply rotation
    X = reshape(newPoints(1, :), size(X, 1), []);  % Re-format into matrix form
    Y = reshape(newPoints(2, :), size(Y, 1), []);
    Z = reshape(newPoints(3, :), size(Z, 1), []);
    
    % Translate the cylinder so that v0 is at the first base center
    X = X + v0(1);
    Y = Y + v0(2);
    Z = Z + v0(3);

    % Plot the cylinder
    surf(X, Y, Z, 'EdgeColor', 'none', 'FaceColor', faceColor, 'FaceAlpha',faceopacity);
    xlabel('X-axis'); ylabel('Y-axis'); zlabel('Z-axis');
end
