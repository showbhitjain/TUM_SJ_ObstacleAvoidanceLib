function isCrossed = checkJointLimits(currentAngles, minLimits, maxLimits)
    % currentAngles, minLimits, and maxLimits are arrays of the same length
    % They represent the joint angles and their corresponding minimum and maximum limits
    
    % Check if any joint angle is less than the minimum limit or greater than the maximum limit
    lowerCrossed = currentAngles < minLimits;  % Check for lower limit violation
    upperCrossed = currentAngles > maxLimits;  % Check for upper limit violation
    
    % Combine the results to find any limit violation
    isCrossed = any(lowerCrossed | upperCrossed);
    
    if isCrossed
        %fprintf('Warning: One or more joint limits have been crossed.\n');
        % Optionally, find which joints have crossed their limits for detailed debugging:
        for i = 1:length(currentAngles)
            if lowerCrossed(i)
                %fprintf('Joint %d is below the minimum limit: %f < %f\n', i, currentAngles(i), minLimits(i));
            end
            if upperCrossed(i)
                %fprintf('Joint %d is above the maximum limit: %f > %f\n', i, currentAngles(i), maxLimits(i));
            end
        end
    else
        %fprintf('All joints are within their limits.\n');
    end
end
