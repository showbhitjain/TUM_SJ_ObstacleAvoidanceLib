function s = smoothing_constraint_scheme2(d, d1, d2, k)
    
%d1 inner threshold 
%d2 outer threshold

% %k: The efficiency of the manipulator to change the velocity of
% the critical point C when avoiding obstacles increases with the
% decrease of k, but the burden of the manipulator will increase
% at the same time. k should be d1 +d2/d2 <= k <= d1 +d2/d1 otherwise error  


% Calculate d0 based on d1, d2, and k
    d0 = (d1 + d2) / k;
    
    % Check if k is within the valid range, and correct it if not
    k_min = (d1 + d2) / d2;
    k_max = (d1 + d2) / d1;

    % If k is not valid, set it to the closest bound
    if k < k_min
        k = k_min;
        d0 = (d1 + d2) / k; % Recalculate d0 with the new k value
    elseif k > k_max
        k = k_max;
        d0 = (d1 + d2) / k; % Recalculate d0 with the new k value
    end

    % Determine the value of s based on the condition of d
    if d >= d2
        s = 1;
    elseif d > d0 && d < d2
        s = cos(pi * (d - d0) / (d2 - d0) + pi);
    elseif d <= d0
        s = -1;
    else
        % This part of the code should never be reached with valid inputs
        s = NaN; % Return NaN if the input does not match any condition
    end
end