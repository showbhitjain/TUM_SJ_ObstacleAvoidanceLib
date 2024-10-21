function lineSegment = createLineSegment(v0, v1, radius)
    % Validate inputs (optional, but good practice)
    if length(v0) ~= 3 || length(v1) ~= 3
        error('Start and end points must be 3-dimensional vectors.');
    end
    if radius <= 0
        error('Radius must be positive.');
    end
    
    % Create the edge vector e
    e = v1 - v0;

    % Define the struct
    lineSegment = struct(...
        'StartPoint', v0, ...
        'EndPoint', v1, ...
        'Edge', e, ...
        'Radius', radius ...
    );
end
