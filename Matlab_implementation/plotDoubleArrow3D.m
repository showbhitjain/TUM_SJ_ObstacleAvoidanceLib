
%do a hold off after this function if you dont want to continue ploting on
%same figure
function plotDoubleArrow3D(v0, v1, varargin)
    % Vector components from v0 to v1
    u = v1(1) - v0(1);
    v = v1(2) - v0(2);
    w = v1(3) - v0(3);
    
    % Plot arrow from v0 to v1
    quiver3(v0(1), v0(2), v0(3), u, v, w, 'AutoScale', 'off', varargin{:});
    hold on;
    
    % Plot arrow from v1 to v0 (reverse)
    quiver3(v1(1), v1(2), v1(3), -u, -v, -w, 'AutoScale', 'off', varargin{:});
    
end
