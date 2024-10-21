function plotLineSweptSpheres(linesegments)
    hold on; axis equal; grid on;
    xlabel('X-axis'); ylabel('Y-axis'); zlabel('Z-axis');
    xlim('auto');
    ylim('auto');
    zlim('auto');
    for i = 1:length(linesegments)
        % Plot cylinder for 'a' segment
        if ~any(isnan(linesegments(i).aSegmentV0)) && ~any(isnan(linesegments(i).aSegmentV1))
            plotCylinder(linesegments(i).aSegmentV0', linesegments(i).aSegmentV1', linesegments(i).radius);
            plotSphere(linesegments(i).aSegmentV0,linesegments(i).radius);
        end

        % Plot cylinder for 'd' segment
        if ~any(isnan(linesegments(i).dSegmentV0)) && ~any(isnan(linesegments(i).dSegmentV1))
            plotCylinder(linesegments(i).dSegmentV0', linesegments(i).dSegmentV1', linesegments(i).radius);
            plotSphere(linesegments(i).dSegmentV0,linesegments(i).radius);
        end
    end

    hold off;
end
