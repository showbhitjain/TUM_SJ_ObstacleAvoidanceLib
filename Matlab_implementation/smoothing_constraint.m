function s_d = smoothing_constraint(d,d1,d2)
%d2 = d_influence
%d1 = d_stop
% Function to calculate s(d)
s_d = NaN; %default value

if d >= d2
            s_d = 1;
        elseif d > d1
            s_d = sin((pi / 2) * (d - d1) / (d2 - d1))^2;
        elseif d<= d1
            s_d = 0;
        elseif any(isnan(d)) == true
            s_d = NaN;
    end
 end

