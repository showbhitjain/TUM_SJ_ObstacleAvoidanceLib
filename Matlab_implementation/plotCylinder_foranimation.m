function h = plotCylinder_foranimation(v0, v1, radius, varargin)
    p = inputParser;
    addParameter(p, 'Parent', gca, @(x) isgraphics(x, 'axes'));
    addParameter(p, 'FaceColor', [1, 0, 0], @(x) isnumeric(x) && length(x)==3);
    addParameter(p, 'FaceAlpha', 0.1, @(x) isnumeric(x) && x >= 0 && x <= 1);
    parse(p, varargin{:});

    numPoints = 20;
    [X, Y, Z] = cylinder(radius, numPoints);
    Z = Z * norm(v1 - v0);  % Scale Z by the length of the cylinder

    direction = (v1 - v0) / norm(v1 - v0);
    up = [0; 0; 1];
    axis = cross(up, direction);
    angle = acos(dot(up, direction));

    if norm(axis) ~= 0
        axis = axis / norm(axis);  % Normalize the axis
        axang = [axis', angle];    % Concatenate to form the axis-angle array, ensuring all elements are correctly dimensioned
        R = axang2rotm(axang);    % Create the rotation matrix
    else
        R = eye(3);  % The direction is already aligned with the Z-axis
    end

    newPoints = R * [X(:), Y(:), Z(:)]';  % Apply rotation
    X = reshape(newPoints(1, :), size(X, 1), []);  % Re-format into matrix form
    Y = reshape(newPoints(2, :), size(Y, 1), []);
    Z = reshape(newPoints(3, :), size(Z, 1), []);

    X = X + v0(1);
    Y = Y + v0(2);
    Z = Z + v0(3);

    h = surf(X, Y, Z, 'EdgeColor', 'none', 'FaceColor', p.Results.FaceColor, 'FaceAlpha', p.Results.FaceAlpha, 'Parent', p.Results.Parent);
end