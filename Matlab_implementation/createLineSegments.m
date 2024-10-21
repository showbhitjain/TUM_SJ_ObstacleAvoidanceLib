
%jointangles shouls 1xnumberofjoints
%robot is rigidbodytree
%mdhparams is mdhparams matrix defined 
% radius defines radius of each link and radius vector dimension =  1xnumberofjoints 
function linesegments = createLineSegments(mdhparams,robot,radius,jointangles)
    % Initialize the struct array
    defaultVector = [NaN NaN NaN];
    linesegments = repmat(struct('aSegmentV0', defaultVector, 'aSegmentV1', defaultVector, 'dSegmentV0', defaultVector, 'dSegmentV1', defaultVector, 'radius', NaN), size(mdhparams, 1), 1);
    
    % Assume home configuration for simplicity
    %homeConfiguration_1 = homeConfiguration(robot);
    
    for i = 1:size(mdhparams, 1)
        % Get the transformation from the base to the previous link's coordinate frame
        if i == 1
            prevTransform = eye(4); % The base frame for the first link
        else
            prevTransform = getTransform(robot, jointangles, sprintf('robot_link%d',int32(i-1)), 'base');
        end
         
        %Initialize aTransform here to handle any execution path safely
        aTransform = eye(4);

        % Check if 'a' is not zero and calculate 'a' segment
        if mdhparams(i, 1) ~= 0
        
        % Initialize v0 for 'a' segment
        linesegments(i).aSegmentV0 = prevTransform(1:3, 4)';
        
        %Initialize v1 for 'a' segment
        aTransform = prevTransform  *  trvec2tform([mdhparams(i,1),0,0]) ;%* trotx(mdhparams(i, 2)) * transl([mdhparams(i, 1), 0, 0]);
        linesegments(i).aSegmentV1 = aTransform(1:3, 4)';
        %disp(aTransform);
        end
       
        %eul2tform([0,0,-mdhparams(i,2)])
        % Initialize v0 for 'd' segment

        
        % Check if 'd' is not zero and 'a' is not zero calculate 'd' segment
        if mdhparams(i, 1) ~= 0 && mdhparams(i,3) ~= 0
             % Initialize v0 for 'd' segment
            linesegments(i).dSegmentV0 = linesegments(i).aSegmentV1;
            %Initialize v1 for 'd' segment
            dTransform = aTransform * eul2tform([0,0,mdhparams(i,2)])* trvec2tform([0, 0, mdhparams(i, 3)]);
            linesegments(i).dSegmentV1 = dTransform(1:3, 4)';
        % Check if 'd' is not zero and 'a' is zero calculate 'd' segment    
        elseif mdhparams(i, 1) == 0 && mdhparams(i,3) ~= 0
            % Initialize v0 for 'd' segment
            linesegments(i).dSegmentV0 = prevTransform(1:3, 4)';
            %Initialize v1 for 'd' segment
            dTransform =  prevTransform * eul2tform([0,0,mdhparams(i,2)])* trvec2tform([0, 0, mdhparams(i, 3)]);
            linesegments(i).dSegmentV1 = dTransform(1:3, 4)';
        end
        linesegments(i).radius = radius(i);
    end

    

end


