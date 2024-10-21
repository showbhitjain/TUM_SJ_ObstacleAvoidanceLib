%add critical point to a robot link
%{
robot = rigidbodytree of robot with links
relative_distance_to_vo = distance between link frame and
critcal point on the link 
%}

function body_critical = generate_Criticalbody(critical_bodyname,critical_joint_name,relative_distance_to_vo)

body_critical = rigidBody(critical_bodyname);
joint_critical = rigidBodyJoint(critical_joint_name,'fixed');
relative_distance_to_vo_tform = trvec2tform(relative_distance_to_vo);
setFixedTransform(joint_critical,relative_distance_to_vo_tform);
body_critical.Joint = joint_critical;

end