figure;
v0 = [0 0 0];
v1 = [0 0 1];
sphere_center = [2 0 -2];
plotCylinder([0 0 0]',[0 0 1]', 0.5)
hold on;
plotSphere(v0,0.5);
plotSphere(v1,0.5);

plotSphere(sphere_center,0.5);
[s,dist,dist_v1,dist_v0] = calculate_distance_lss_pss(v0,v1,0.5,sphere_center ,0.5)

plotline(dist_v0,dist_v1);
    hold off;