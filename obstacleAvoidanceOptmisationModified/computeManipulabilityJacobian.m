function Jm_transposed = computeManipulabilityJacobian(joint_values)
    % Compute the Jacobian and Hessian first
    %[~, J, H] = computeKinematics(joint_values, mdhparams); because of syms was too slow
    
    %J = Jacobi_final_link1(joint_values); %for code generation
    %H = Hessian_final_link(joint_values); %for code generation
    J = Jacobi_final_link(joint_values);
    H = Hessian_final_link(joint_values);
    % Compute the manipulability measure 'm'
    JJt = J * J';
    m = sqrt(det(JJt));
    
    % Compute the manipulability Jacobian 'Jm'
    n = size(J, 2);  % Number of joints
    Jm_transposed = zeros(1, n);  % Initialize manipulability Jacobian transposed
    
    % vec operation implemented as a function for convenience
    vec = @(X) reshape(X, [], 1);
     
    % Pre-compute the inverse of JJt
    inv_JJt = inv(JJt); %
    % Pre-compute the pseudo-inverse of JJt
    
    %pseudo_inv_JJt = pinv(JJt);

    for i = 1:n
        Hi = H(:,:,i);
        term1 = vec(J * Hi')' * vec(inv_JJt);
        Jm_transposed(i) = m * term1;  % Equation (11)
    end
end