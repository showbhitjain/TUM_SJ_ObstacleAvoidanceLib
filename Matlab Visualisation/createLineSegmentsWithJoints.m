%jointangles shouls 1xnumberofjoints
%robot is rigidbodytree
%mdhparams is mdhparams matrix defined
% radius defines radius of each link and radius vector dimension =  1xnumberofjoints
function [finalLinkBox,linesegments] = createLineSegmentsWithJoints(mdhparams,robot,radius,radiusJoints,finalLinkDimension,jointangles)
% Initialize the struct array
defaultVector = [NaN NaN NaN];
defaultOrientation = NaN(3);
numberLinksExceptFinalLink = size(mdhparams, 1);
linesegments = repmat(struct('aSegmentV0', defaultVector, 'aSegmentV1', defaultVector, 'dSegmentV0', defaultVector, 'dSegmentV1', defaultVector, 'radius', NaN,'radiusJoint',NaN), numberLinksExceptFinalLink, 1);
finalLinkBox = struct('center',defaultVector,'dimension',defaultVector,'orientationMatrix',defaultOrientation,'finalV0',defaultVector,'finalV1',defaultVector);

% Assume home configuration for simplicity

% homeConfiguration_1 = homeConfiguration(robot);

for i = 1:numberLinksExceptFinalLink
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
    linesegments(i).radiusJoint = radiusJoints(i);
end

%final Link Box
finalLinkBox.dimension = finalLinkDimension;

transformationStartFinalLink = getTransform(robot, jointangles, sprintf('robot_link%d',int32(numberLinksExceptFinalLink)), 'base');
finalLinkBox.finalV0 = transformationStartFinalLink(1:3, 4)';

tcpTransform =  getTransform(robot,jointangles,'Gripper_TCP','base');
finalLinkBox.orientationMatrix = tcpTransform(1:3,1:3);
finalLinkBox.finalV1 = tcpTransform(1:3, 4)';

distanceVector = finalLinkBox.finalV1  - finalLinkBox.finalV0 ;
distanceVectorNormalized = distanceVector / norm(distanceVector);

% Compute dot products between the normalized distance vector and each axis of the TCP orientation
dotProductX = dot(distanceVectorNormalized, tcpTransform(1:3, 1));
dotProductY = dot(distanceVectorNormalized, tcpTransform(1:3, 2));
dotProductZ = dot(distanceVectorNormalized, tcpTransform(1:3, 3));

tolerance = 1e-4;  % tolerance for near-zero comparisons

% Determine along which axis the final link extends and compute the center
if ( (dotProductX ~= 0) && (abs(dotProductY) < tolerance) && (abs(dotProductZ) < tolerance) )
    % Final link is along the X axis
    finalLinkBox.center = finalLinkBox.finalV0 + distanceVectorNormalized * (finalLinkBox.dimension(1) / 2);
    

elseif ( (abs(dotProductX) < tolerance) && (dotProductY ~= 0) && (abs(dotProductZ) < tolerance) )
    % Final link is along the Y axis
    finalLinkBox.center = finalLinkBox.finalV0 + distanceVectorNormalized * (finalLinkBox.dimension(2) / 2);
   

elseif ( (abs(dotProductX) < tolerance) && (abs(dotProductY) < tolerance) && (dotProductZ ~= 0) )
    % Final link is along the Z axis
    finalLinkBox.center = finalLinkBox.finalV0 + distanceVectorNormalized * (finalLinkBox.dimension(3) / 2);
    

else
    % If none of the above conditions are met, you might want to handle this case.
    error('Unable to determine the dominant direction for the final link.');
end


end


