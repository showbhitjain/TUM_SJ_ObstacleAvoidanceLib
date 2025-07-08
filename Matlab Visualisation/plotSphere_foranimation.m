function h = plotSphere_foranimation(center, radius, varargin)
   p = inputParser;
    addParameter(p, 'Parent', gca, @(x) isgraphics(x, 'axes'));
    addParameter(p, 'FaceColor', [0, 0, 1], @(x) isnumeric(x) && length(x)==3);
    addParameter(p, 'FaceAlpha', 0.1, @(x) isnumeric(x) && x >= 0 && x <= 1);
    parse(p, varargin{:});

    [X, Y, Z] = sphere;
    X = X * radius;
    Y = Y * radius;
    Z = Z * radius;

    X = X + center(1);
    Y = Y + center(2);
    Z = Z + center(3);

    h = surf(X, Y, Z, 'EdgeColor', 'none', 'FaceColor', p.Results.FaceColor, 'FaceAlpha', p.Results.FaceAlpha, 'Parent', p.Results.Parent);
    axis equal; grid on;
    xlabel('X-axis');
    ylabel('Y-axis');
    zlabel('Z-axis');
end