function h = plotBox_forAnimation(box_center,box_dims,orientationRotationMatrix,verbose,varargin)

%% 2. Plot an Oriented Box
% Box parameters
%box_center = [-4, 0, 2];    % Center of the box
%box_dims   = [2, 3, 4];     % Complete dimensions along box's local x,y,z
%orientationRotationMatrix = Define the box orientation as a rotation matrix



p = inputParser;
    addParameter(p, 'Parent', gca, @(x) isgraphics(x, 'axes'));
    addParameter(p, 'FaceColor', [1, 0, 0], @(x) isnumeric(x) && length(x)==3);
    addParameter(p, 'FaceAlpha', 0.1, @(x) isnumeric(x) && x >= 0 && x <= 1);
    parse(p, varargin{:});


% 
R_box = orientationRotationMatrix; 

% Define 8 vertices of the box in its local frame (centered at origin)
lx = box_dims(1)/2; ly = box_dims(2)/2; lz = box_dims(3)/2;
localVertices = [ -lx, -ly, -lz;
                   lx, -ly, -lz;
                   lx,  ly, -lz;
                  -lx,  ly, -lz;
                  -lx, -ly,  lz;
                   lx, -ly,  lz;
                   lx,  ly,  lz;
                  -lx,  ly,  lz];

% Transform vertices to global coordinates
globalVertices_box = (R_box * localVertices')' + box_center;

% Define the 6 faces of the box (indices into vertices)
faces_box = [1 2 3 4;    % bottom face
             5 6 7 8;    % top face
             1 2 6 5;    % side face
             2 3 7 6;
             3 4 8 7;
             4 1 5 8];

% Plot the box using patch
h = patch('Vertices', globalVertices_box, 'Faces', faces_box,'Parent', p.Results.Parent, 'FaceColor', p.Results.FaceColor, 'FaceAlpha', p.Results.FaceAlpha);

% Plot the box coordinate frame (box local axes)
if verbose == true
    axisLength = max(box_dims)*0.8;
    x_axis_local = [axisLength, 0, 0]';
    y_axis_local = [0, axisLength, 0]';
    z_axis_local = [0, 0, axisLength]';
    x_axis_global = R_box * x_axis_local;
    y_axis_global = R_box * y_axis_local;
    z_axis_global = R_box * z_axis_local;
    quiver3(box_center(1), box_center(2), box_center(3), x_axis_global(1), x_axis_global(2), x_axis_global(3), 'Color','r','LineWidth',2, 'MaxHeadSize',0.5);
    quiver3(box_center(1), box_center(2), box_center(3), y_axis_global(1), y_axis_global(2), y_axis_global(3), 'Color','g','LineWidth',2, 'MaxHeadSize',0.5);
    quiver3(box_center(1), box_center(2), box_center(3), z_axis_global(1), z_axis_global(2), z_axis_global(3), 'Color','b','LineWidth',2, 'MaxHeadSize',0.5);
    text(box_center(1)+x_axis_global(1), box_center(2)+x_axis_global(2), box_center(3)+x_axis_global(3), 'X','FontSize',12,'Color','r');
    text(box_center(1)+y_axis_global(1), box_center(2)+y_axis_global(2), box_center(3)+y_axis_global(3), 'Y','FontSize',12,'Color','g');
    text(box_center(1)+z_axis_global(1), box_center(2)+z_axis_global(2), box_center(3)+z_axis_global(3), 'Z','FontSize',12,'Color','b');
end
end