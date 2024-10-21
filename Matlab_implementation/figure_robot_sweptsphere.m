close; 
show(frankarobot,Initalconfig,'Frames','off','PreservePlot',false);
plotLineSweptSpheres(linesegments)
Spherecenter = [0.3, 0.4,0.7];
plotSphere(Spherecenter,0.1,[0 0 1],0.3)
[s,dist,dist_v1,dist_v0] = calculate_distance_lss_pss(linesegments(5).dSegmentV0,linesegments(5).dSegmentV1,linesegments(5).radius,Spherecenter,0.1);

plotline(dist_v0,dist_v1,'Color','k');

xlim([-0.6 0.6]);
ylim([-0.6 0.6]);


