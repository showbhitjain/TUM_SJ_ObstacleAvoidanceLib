function optimizedVisualizeRobot(rbtName,jointAnglesMatrix, waypoints, mode)
    persistent robot ax trajPlot wayptPlot

    if isempty(robot)
        close all;
        figure;
        robot = evalin('base', rbtName);
        robot.DataFormat = 'row';

        % Setup the environment once
        ax = show(robot, robot.homeConfiguration, 'Frames', 'off');
        hold on;
        title('Robot Trajectory Visualization');
        wayptPlot = scatter3(waypoints(1,:), waypoints(2,:), waypoints(3,:), 'ro', 'LineWidth', 2);
        xlim([-1 1]), ylim([-1 1]), zlim([0 1.2]);
    end

    % Plot trajectory only if mode is 2 and it's the first function call or waypoints have changed
    if mode == 2 && (isempty(trajPlot) || ~isequal(waypoints, wayptPlot.XData))
        trajPlot = plot3(ax, waypoints(1,:), waypoints(2,:), waypoints(3,:), 'b.-');
    end

    % Optimize the loop by directly updating robot pose without re-plotting static elements
    for idx = 1:size(jointAnglesMatrix, 1)
        jointAngles = jointAnglesMatrix(idx, :);

        % Update robot pose efficiently
        show(robot, jointAngles, 'PreservePlot', true, 'Frames', 'off', 'Parent', ax);
        drawnow limitrate; % Improves performance by limiting the draw rate
    end
end
