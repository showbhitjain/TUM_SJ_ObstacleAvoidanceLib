function plotarrow(v0, v1, varargin)
    % Calculate the differences
    u = v1(1) - v0(1);
    v = v1(2) - v0(2);
    w = v1(3) - v0(3);
    
    % Plot the arrow
    quiver3(v0(1), v0(2), v0(3), u, v, w, 'AutoScale', 'off', varargin{:});
end
