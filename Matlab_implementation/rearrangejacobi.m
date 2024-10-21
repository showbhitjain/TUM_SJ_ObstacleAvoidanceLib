function [jacobi] = rearrangejacobi(J,number_of_Joints)
%use only if the robotics toolbox jacobi J is used as an input


J_i = zeros(6,number_of_Joints);
J_i(1:3,1:end) = J(4:end,1:end);
J_i(4:end,1:end) = J(1:3,1:end);
jacobi = J_i;


end
