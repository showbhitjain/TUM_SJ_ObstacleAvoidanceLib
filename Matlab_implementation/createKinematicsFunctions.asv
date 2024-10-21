function createKinematicsFunctions(mdhparams)
    % Define symbolic variables for the joint angles
    syms theta [1 7] real; % Adjust the '7' if the number of joints is different

    % Extract DH parameters
    a = mdhparams(:,1);
    alpha = mdhparams(:,2);
    d = mdhparams(:,3);

    % Compute the transformation matrices and the overall transformation matrix
    T = sym(eye(4));
    for i = 1:length(theta)
        T_i = transformDH(a(i), alpha(i), d(i), theta(i));
        T = T * T_i;
    end
    
    % Initialize Jacobian matrices
    Jv = sym(zeros(3,length(theta)));
    Jw = sym(zeros(3,length(theta)));
    p = T(1:3, 4); % Position of the end effector

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
   
 % Simplify the Jacobian matrix
    for i = 1:size(J, 1)
        for j = 1:size(J, 2)
            J(i, j) = simplify(J(i, j), 'Steps', 50); % Adjust 'Steps' as necessary
        end
    end
  


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
    
     % Simplify the Hessian tensor
    for i = 1:6
        for j = 1:n
            for k = 1:n
                Hessian(i, j, k) = simplify(Hessian(i, j, k), 'Steps', 50); % Adjust 'Steps' as necessary
            end
        end
    end

    % Create MATLAB functions from symbolic expressions
    T_final_link = matlabFunction(T, 'Vars', {theta}, 'File', 'T_final_link1');
    Jacibi_final_link = matlabFunction(J, 'Vars', {theta}, 'File', 'Jacobi_final_link1');
    Hessian_final_link = matlabFunction(Hessian, 'Vars', {theta}, 'File', 'Hessian_final_link1');
    
    disp('Functions created successfully!');
end