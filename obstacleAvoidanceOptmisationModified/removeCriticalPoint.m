function removeCriticalPoint(robot, name)
    

% Check if body exists in the robot before removing
    if any(strcmp(robot.BodyNames, name))
        removeBody(robot, name);
    end


end


