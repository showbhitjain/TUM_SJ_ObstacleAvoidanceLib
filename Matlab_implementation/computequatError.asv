
%qd and qc must be 1x4
function qe = computequatError(qd, qc)
    %Ensure the quaternions are normalized
    %     qd = qd / norm(qd);
    % qc = qc / norm(qc);

     
     % Compute the conjugate of the current orientation quaternion
    qc_conj = [qc(1), -qc(2), -qc(3), -qc(4)];
     % Compute the quaternion error
    qerror = quatmultiply(qc_conj, qd);
    qe = qerror;
    
    % 
    % qc_conj = [qc(1), -qc(2), -qc(3), -qc(4)];
    % 
    % % Compute the quaternion error
    % qerror = quatmultiply(qd, qc_conj);
    % 
    % axang = quat2axang(qerror);
    % 
    % qe_ax = (axang(4) * axang(1:3))';
    % qe = T_i(1:3, 1:3) * qe_ax;
end

   % 
   %   eta_d = qd(1);
   %  eta = qc(1);
   %  q = qc(2:4);
   %  q_d = qd(2:4);
   % 
   % qerror = -eta_d * q + eta * q_d - cross(q_d, q);
   % 
   %  qe = T_i(1:3, 1:3) * (qerror)';



