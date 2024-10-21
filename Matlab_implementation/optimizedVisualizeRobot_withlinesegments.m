function optimizedVisualizeRobot_withlinesegments(rbtName_robot_visualisation, robot_calculated, jointAnglesMatrix, waypoints, mode, obstacles, mdhparams, radius)
    persistent robot ax trajPlot wayptPlot elapsedText obstaclePLot lineSweptPlot

    % Check number of input arguments
    if nargin < 5
        obstacles = [];
    end

    % Initialize the robot model if it hasn't been done yet
    if isempty(robot)
        robot = evalin('base', rbtName_robot_visualisation);
        robot.DataFormat = 'row';
    end

    % Setup figure and axes if not already set
    if isempty(ax) || ~isgraphics(ax, 'axes')
        clf;  % Clear the current figure window to start fresh
        figure;
        ax = show(robot, robot.homeConfiguration, 'Frames', 'off');
       
        title('Robot Visualization');
        xlim([-1 1]), ylim([-1 1]), zlim([0 1.2]);
        wayptPlot = [];
        trajPlot = [];
        obstaclePLot = [];
        lineSweptPlot = [];
        elapsedText = text(0.05, 0.95, 'Elapsed Time: 0s', 'Units', 'normalized',...
                           'VerticalAlignment', 'top', 'FontSize', 12, 'Parent', ax);
    end
    hold on;
    % Plot waypoints if available
     if isempty(wayptPlot) && ~isempty(waypoints)
        
         wayptPlot = scatter3(waypoints(1,:), waypoints(2,:), waypoints(3,:), 'o', 'MarkerEdgeColor', [0, 1, 0], 'LineWidth', 2);
    end
    % Plot trajectory only if mode is 2 and it's the first function call or waypoints have changed
    if mode == 2 && (isempty(trajPlot) || ~isequal(waypoints, wayptPlot.XData))
        
       % trajPlot = plot3(waypoints(1,:), waypoints(2,:), waypoints(3,:), '.-', 'Color', [0.58, 0, 0.83]);
        trajPlot = plot3(waypoints(1,:), waypoints(2,:), waypoints(3,:), '.-', 'Color', [0, 0, 0]);
    end


    if isempty(obstaclePLot) && ~isempty(obstacles)
        obstaclePLot = gobjects(length(obstacles), 1); % Initialize array to hold plot handles
        for i = 1:length(obstacles)
            obstaclePLot(i) = plotSphere(obstacles(i).center, obstacles(i).dimensions(1), [0 0 1], 0.5);
        end
    end
    realTimeStart = tic; % Restart the timer
    idx = 1;

    while idx <= size(jointAnglesMatrix, 1)
        realTimeElapsed = toc(realTimeStart) * 1000; % Convert to milliseconds
        expectedTime = idx; % Expected time in ms, directly from idx

        if realTimeElapsed > expectedTime
            idxIncrease = floor((realTimeElapsed - expectedTime) / 1000 * 1000); % Example for skipRate of 100 ms
            idx = idx + idxIncrease;
        elseif realTimeElapsed < expectedTime
            pause((expectedTime - realTimeElapsed) / 1000); % Convert ms back to seconds
        end

        if idx > size(jointAnglesMatrix, 1)
            break; % Ensure idx does not exceed matrix size
        end

        jointAngles = jointAnglesMatrix(idx, :);
        show(robot, jointAngles, 'PreservePlot', false, 'Frames', 'off', 'Parent', ax);

        jointAngles_linesegments = jointAngles(1,1:size(mdhparams,1));
        % Calculate new line segments for the current joint configuration
        linesegments = createLineSegments(mdhparams, robot_calculated, radius, jointAngles_linesegments);
        
        % Clear previous line swept spheres before redrawing
       if any(isgraphics(lineSweptPlot))
            delete(lineSweptPlot);
       end



        % Update or plot new line-swept spheres
        lineSweptPlot = plotLineSweptSpheres_foranimation(linesegments, ax);

        xlim([-1 1]);
        ylim([-1 1]);
       zlim([-0.3 1]);
        if ~isgraphics(elapsedText)
            elapsedText = text(0.05, 0.95, '', 'Units', 'normalized', 'VerticalAlignment', 'top', 'FontSize', 12, 'Parent', ax);
        end

        elapsedTime = toc(realTimeStart); % Update elapsed time in seconds
        set(elapsedText, 'String', sprintf('Elapsed Time: %.2fs', elapsedTime));
        
        drawnow limitrate;
        
        idx = idx + 1; % Move to the next frame
    end
    hold off;
end
