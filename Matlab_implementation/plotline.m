%v0 startingpoint of line
%v1 end point of line
function plotline(v0, v1, varargin)
    x = [v0(1), v1(1)];
    y = [v0(2), v1(2)];
    z = [v0(3), v1(3)];
    
    % Plot the line with optional additional arguments
    plot3(x, y, z, 'LineWidth', 2, varargin{:});
end

