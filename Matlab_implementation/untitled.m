function [val, gradient] = objective_function(q)
    val = 1/2 * q*q' + ;
    if (nargout > 1)
        gradient = [0, 0.5 / val];
    end


