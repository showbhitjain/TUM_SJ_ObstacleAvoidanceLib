function hGrp = plotCylinderNew(cyl_center,cyl_axis,cyl_radius,cyl_height,color)
%% 1. Plot a Closed Cylinder
% Cylinder parameters
% cyl_center: Center of the cylinder
% cyl_axis: Normalized axis
% cyl_radius: Cylinder radius
% cyl_height: Cylinder height



% Create cylinder along the z-axis using nPoints around the circle
nPoints = 50;
[Xc, Yc, Zc] = cylinder(cyl_radius, nPoints);
% Scale Z to span the given height, centered at zero
Zc = (Zc - 0.5)*cyl_height;

% Compute rotation matrix to align [0 0 1] with cyl_axis
z_axis = [0, 0, 1];
v = cross(z_axis, cyl_axis);
s = norm(v);
if s < 1e-6
    R_cyl = eye(3);
else
    c = dot(z_axis, cyl_axis);
    vx = [    0   -v(3)   v(2);
           v(3)      0   -v(1);
          -v(2)   v(1)      0];
    R_cyl = eye(3) + vx + vx*vx*((1-c)/(s^2));
end

% Transform cylinder coordinates: rotate and then translate
[nr, nc] = size(Xc);
Xg = zeros(nr, nc); Yg = zeros(nr, nc); Zg = zeros(nr, nc);
for i = 1:nr
    for j = 1:nc
        pt = [Xc(i,j); Yc(i,j); Zc(i,j)];
        pt_rot = R_cyl * pt;
        pt_global = pt_rot + cyl_center';
        Xg(i,j) = pt_global(1);
        Yg(i,j) = pt_global(2);
        Zg(i,j) = pt_global(3);
    end
end

 % individual primitives
    hSurf = surf(Xg, Yg, Zg, 'EdgeColor','none', 'FaceAlpha',0.7,'FaceColor',color);
    hTop  = patch(Xg(end,:), Yg(end,:), Zg(end,:), color, 'EdgeColor','none','FaceAlpha',0.7);
    hBot  = patch(Xg(1,:) , Yg(1,:) , Zg(1,:) , color, 'EdgeColor','none','FaceAlpha',0.7);

    % group them under one parent
    hGrp  = hggroup;
    set([hSurf hTop hBot], 'Parent', hGrp);     % now hGrp is the single handle



end