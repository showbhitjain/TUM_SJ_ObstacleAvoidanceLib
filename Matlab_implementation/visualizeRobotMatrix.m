function visualizeRobotMatrix(rbtName, eeName, jointAnglesMatrix, waypoints, orientations, mode, pauseTime)
    % ... (the rest of the existing code remains the same)

    % Determine the number of trajectory points
    numTrajectoryPoints = size(jointAnglesMatrix, 1);

    for idx = 1:numTrajectoryPoints
        jointAngles = jointAnglesMatrix(idx, :);
        jointAngles = jointAngles';
        % Update the visualization with the current joint angles
        visualizeRobot(rbtName, eeName, jointAngles, waypoints, orientations, mode);

        % Pause for 1ms (or the provided pause time) to update the visualization
        %pause(pauseTime);
    end
end