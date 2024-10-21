function [T_final, J_final,Hessian_final] = computeKinematics(theta_values, mdhparams)
    % Input:
    %   theta_values - Numeric or symbolic array of joint angles
    %   mdhparams - Matrix of modified DH parameters [a, alpha, d, theta]
    % Output:
    %   T_final - Transformation matrix from base to end effector
    %   J_final - Jacobian matrix

    % Define symbolic variables for the joint angles
    syms theta [1 length(theta_values)] real;

    % Extract DH parameters
    a = mdhparams(:,1);
    alpha = mdhparams(:,2);
    d = mdhparams(:,3);

    % Compute the transformation matrices and the overall transformation matrix
    T = sym(eye(4));  % Initialize the transformation matrix
    for i = 1:length(theta)
        T_i = transformDH(a(i), alpha(i), d(i), theta(i));
        T = T * T_i;  % Cumulative product of transformation matrices
        %T = simplify(T);  % Simplify at each step (optional
    end
    
    % Substitute the specific values of theta into the final transformation matrix
    
    
    
    % Initialize Jacobian matrices
    Jv = sym(zeros(3,length(theta)));  % Linear velocity Jacobian
    Jw = sym(zeros(3,length(theta)));  % Angular velocity Jacobian

    % Position and orientation of the end effector
    p = T(1:3, 4);

    % Compute Jacobian using iterative method
    for i = 1:length(theta)
        if i == 1
            Jv(:,i) = cross([0; 0; 1], (p - [0; 0; 0]));
            Jw(:,i) = [0; 0; 1];
        else
            T_current = sym(eye(4));
            for j = 1:i
                T_current = T_current * transformDH(a(j), alpha(j), d(j), theta(j));
            end
            z = T_current(1:3, 3);
            o = T_current(1:3, 4);
            Jv(:,i) = cross(z, (p - o));
            Jw(:,i) = z;
        end
    end

    % Full Jacobian matrix combining both linear and angular parts
    J = [Jv; Jw];
   

  


   % Compute the symbolic Hessian tensor
    n = length(theta);
    Hessian = sym(zeros(6, n, n));
    for i = 1:6
        for j = 1:n
            for k = 1:n
                Hessian(i, j, k) = diff(J(i, j), theta(k));
            end
        end
    end

    
    % Evaluate the Hessian at the given theta values
    Hessian_final = subs(Hessian, theta, theta_values);
    

     J_final = subs(J, theta, theta_values);
     T_final = subs(T, theta, theta_values);

end



