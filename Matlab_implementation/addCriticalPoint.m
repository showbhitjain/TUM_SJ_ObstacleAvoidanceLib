function addCriticalPoint(robot, critical_bodyname, critical_joint_name, relative_distance_to_vo, parentIndex)
 
body_critical = rigidBody(critical_bodyname);
joint_critical = rigidBodyJoint(critical_joint_name,'fixed');
relative_distance_to_vo_tform = trvec2tform(relative_distance_to_vo);
setFixedTransform(joint_critical,relative_distance_to_vo_tform);
body_critical.Joint = joint_critical;
    
if parentIndex == 1
        addBody(robot, body_critical, 'base');
else
        addBody(robot, body_critical, sprintf('robot_link%d', int32(parentIndex - 1)));
end

end



% % Add the critical point as a body
% addCriticalPoint(robot, sprintf('Critical_Point_Link_D_%d%d', int32(i), int32(l)), relative_distance_d_vo_toCrit, i);
            