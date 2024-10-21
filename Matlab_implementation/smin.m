function s_min = smin(J)
    s = svd(J);
    s_min = min(s);
end
