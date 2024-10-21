function plotLineSegments_withcolor(linesegments, varargin)
     hold on; grid on; %hold it for multiple plots
    % view(3);
    xlabel('X-axis'); ylabel('Y-axis'); zlabel('Z-axis'); % Label the axes
    axis equal; % Set equal scaling for all axes
    xlim('auto');
    ylim('auto');
    zlim('auto');

% Set default color as blue [0 0 1]
    defaultColor = [0, 0, 1];
    
    % Check if a color argument is provided
    if nargin > 1
        color = varargin{1};
        % Validate that the color is an RGB triplet
        if ~isnumeric(color) || length(color) ~= 3 || any(color < 0) || any(color > 1)
            error('Color must be an RGB triplet with values between 0 and 1.');
        end
    else
        color = defaultColor;
    end


    % Loop through each linesegment
    for i = 1:length(linesegments)
        % Plot 'a' segment if it exists
        if ~any(isnan(linesegments(i).aSegmentV0)) && ~any(isnan(linesegments(i).aSegmentV1))
            plot3([linesegments(i).aSegmentV0(1), linesegments(i).aSegmentV1(1)], ...
                  [linesegments(i).aSegmentV0(2), linesegments(i).aSegmentV1(2)], ...
                  [linesegments(i).aSegmentV0(3), linesegments(i).aSegmentV1(3)], ...
                  'Color', color, 'LineWidth', 2); % color line for 'a' segment
        %Uncomment the next two lines if you wish to see the radius represented
            %viscircles(linesegments(i).aSegmentV0(1:2), linesegments(i).radius, 'Color', 'b');
        end
        
        % Plot 'd' segment if it exists
        if ~any(isnan(linesegments(i).dSegmentV0)) && ~any(isnan(linesegments(i).dSegmentV1))
            plot3([linesegments(i).dSegmentV0(1), linesegments(i).dSegmentV1(1)], ...
                  [linesegments(i).dSegmentV0(2), linesegments(i).dSegmentV1(2)], ...
                  [linesegments(i).dSegmentV0(3), linesegments(i).dSegmentV1(3)], ...
                  'Color', color, 'LineWidth', 2); % color line for 'd' segment
        %viscircles(linesegments(i).dSegmentV0(1:2), linesegments(i).radius, 'Color', 'r');
        end
        
        % Optionally, plot the radius as a circle around the line (not scaled, just for visual)
        % Uncomment the next two lines if you wish to see the radius represented
        
        
    end
    
    hold off; % Release the figure
end
