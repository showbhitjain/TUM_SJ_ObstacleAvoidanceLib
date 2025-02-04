function optimizedVisualizeRobot_2(rbtName, jointAnglesMatrix, waypoints, mode,obstacles)
    persistent robot ax trajPlot wayptPlot  elapsedText obstaclePLot
    if nargin <5
    obstacles = [];
    
    end
    if isempty(robot)
        % figure;
        robot = evalin('base', rbtName);
        robot.DataFormat = 'row';
    end
    
    
    if isempty(ax) || ~isgraphics(ax)
        clf; %Clear the current figure window to start fresh 
        close;
        figure;
        ax = show(robot, robot.homeConfiguration, 'Frames', 'off');
        %ax = axes; %Create a new axes object
          hold on;
         title('Robot Trajectory Visualization');
         xlim([-1 1]), ylim([-1 1]), zlim([0 1.2]);
        
        %view(3); % Set the view to 3D
        %xlim(ax, [-1 1]), ylim(ax, [-1 1]), zlim(ax, [0 1.2]);
        %title(ax, 'Robot Trajectory Visualization');
        %hold(ax, 'on');
        % robot = evalin('base', rbtName);
        % robot.DataFormat = 'row';
        
        % Reinitialize all graphics objects
        
        wayptPlot = [];
        trajPlot = [];
        obstaclePLot = [];
        elapsedText = text(0.05, 0.95, 'Elapsed Time: 0s', 'Units', 'normalized', ...
                             'VerticalAlignment', 'top', 'FontSize', 12, 'Parent', ax);
     end
   
    if isempty(wayptPlot) && ~isempty(waypoints)
        wayptPlot = scatter3(waypoints(1,:), waypoints(2,:), waypoints(3,:), 'o', 'MarkerEdgeColor', [0, 1, 0], 'LineWidth', 2);
    end
    % Plot trajectory only if mode is 2 and it's the first function call or waypoints have changed
    if mode == 2 && (isempty(trajPlot) || ~isequal(waypoints, wayptPlot.XData))
        trajPlot = plot3(waypoints(1,:), waypoints(2,:), waypoints(3,:), '.-', 'Color', [0.58, 0, 0.83]);
    end
   if isempty(obstaclePLot) && ~isempty(obstacles)
       for i=1:length(obstacles)
       plotSphere(obstacles(i).center,obstacles(i).dimensions(1),[0 0 1],0.5)
       xlim([-1 1]), ylim([-1 1]), zlim([0 1.2]);
       end
   end   
    realTimeStart = tic; % Restart the timer


     %totalElapsedTime = 0;
         idx = 1;
    while idx <= size(jointAnglesMatrix, 1)
        realTimeElapsed = toc(realTimeStart) * 1000; % Convert to milliseconds
        expectedTime = idx; % Expected time in ms, directly from idx

        if realTimeElapsed > expectedTime
            % Calculate how much to increase idx based on the time difference
            idxIncrease = floor((realTimeElapsed - expectedTime) / 1000 * 1000); % Example for skipRate of 100 ms
            idx = idx + idxIncrease;
        elseif realTimeElapsed < expectedTime
            % Pause for the difference to slow down the visualization
            pause((expectedTime - realTimeElapsed) / 1000); % Convert ms back to seconds
        end

        if idx > size(jointAnglesMatrix, 1)
            break; % Ensure idx does not exceed matrix size
        end

        jointAngles = jointAnglesMatrix(idx, :);
        show(robot, jointAngles, 'PreservePlot', false, 'Frames', 'off', 'Parent', ax);

        elapsedTime = toc(realTimeStart); % Update elapsed time in seconds
        set(elapsedText, 'String', sprintf('Elapsed Time: %.2fs', elapsedTime));
        drawnow limitrate;
        
        idx = idx + 1; % Move to the next frame
    end
end




