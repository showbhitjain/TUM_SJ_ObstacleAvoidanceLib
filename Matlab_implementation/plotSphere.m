function h = plotSphere(center, radius,faceColor,faceopacity)
hold on; 
if nargin < 3
    % If faceColor is not specified, use a default color
    faceColor = [1, 0, 0]; % Default to red
    faceopacity = 0.3;
 end    

% Generate a unit sphere
    [X, Y, Z] = sphere;
    
    % Scale it to the given radius
    X = X * radius;
    Y = Y * radius;
    Z = Z * radius;
    
    % Translate the sphere to the desired center point
    X = X + center(1);
    Y = Y + center(2);
    Z = Z + center(3);
    
    % Plot the sphere
    h= surf(X, Y, Z, 'EdgeColor', 'none','FaceColor',faceColor, 'FaceAlpha', faceopacity);
    
    % Adjust the appearance of the plot
    axis equal; % Equal aspect ratio for all axes
    grid on; % Turn on the grid
    xlabel('X-axis');
    ylabel('Y-axis');
    zlabel('Z-axis');

end

% Example usage:
% center = [1, 2, 3]; % Replace with your x, y, z values
% radius = 5; % Replace with your desired radius
% plotSphere(center, radius);