%critical point transformed from worl frame to the link frame on which the
%critical point lies

function relative_distance_link_toCrit = relative_distance_in_linkframe(T_basetolink,criticalpointinworld)
% Compute the difference in coordinates between the critical point and aSegmentV0
delta_v_homogeneous = [(criticalpointinworld)'; 1];

% Transform the difference vector to the i-1 link's coordinate frame
relative_distance_link_toCrit_hom = T_basetolink * delta_v_homogeneous;

% Convert back to Cartesian coordinates by removing the homogeneous component
relative_distance_link_toCrit = relative_distance_link_toCrit_hom(1:3)';
end

