function [body, joint] = createRigidBodyAndJoint(name, mdhParamsRow, homePosition, positionLimits)
    body = rigidBody(name);
    joint = rigidBodyJoint([name '_joint'], 'revolute');
    joint.HomePosition = homePosition;
    joint.PositionLimits = positionLimits;
    setFixedTransform(joint, mdhParamsRow, 'mdh');
    body.Joint = joint;
end