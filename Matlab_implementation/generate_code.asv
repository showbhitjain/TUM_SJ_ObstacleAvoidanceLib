% Create a configuration object for C++ code generation
cfg = coder.config('lib', 'ecoder', true); % 'lib' for static library, use 'dll' for dynamic library

% Specify the target language as C++
cfg.TargetLang = 'C++';

% Define the variable-size input type for obstacles
obstacle_type = struct('type', int32(0), 'center', zeros(1,3), 'dimensions', zeros(1,3), 'orientation', zeros(1,4), 'axis', 0.0);
obstacle_array_type = coder.typeof(obstacle_type, [1 Inf], [0 1]);

% Generate C++ code
codegen -config cfg matlab_obstacle_avoidance_for_code_gen -args {obstacle_array_type}
