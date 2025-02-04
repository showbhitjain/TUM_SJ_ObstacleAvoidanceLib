
%lss = line swepr sphere
%pss = point swept sphere
%lss_v0 = linesegment v0 (row)
%v1 = linesegment v1  (row)
%lss_radius = linesegment_radius
%sphere_center (row)
function [s,dist,dist_v1,dist_v0] = calculate_distance_lss_pss(lss_v0,lss_v1,lss_radius,sphere_center,sphere_radius)
%CALCULATE_DISTANCE Summary of this function goes here
%   Detailed explanation goes here
d_l = (lss_v1 - lss_v0)'; %linesegment length (distance between vertices of lss) e

d_plss_v0 = (sphere_center - lss_v0)'; %w

%d_l should be column
%d_plss_v0 should be column
%Projection of d_plss_v0 on linsegment 
d_projection = d_plss_v0' * d_l;

if d_projection <= 0 
s = 0;
elseif d_projection >= ((d_l')*d_l)
    s = 1;
else 
    s = (d_plss_v0' * d_l) / (d_l' * d_l);
end
dist = lss_v0' + s * d_l - sphere_center'; %from pss to lss including radius
dist = dist';
dist_normalized = dist/norm(dist);
distance_Scaler = norm(dist) - lss_radius - sphere_radius;
dist = distance_Scaler* dist_normalized;
%dist_v1 = lss_v0' + s *d_l ;
dist_v1 = (lss_v0' + s *d_l)' - dist_normalized * lss_radius ;

dist_v0 = sphere_center + sphere_radius*dist_normalized;
end

