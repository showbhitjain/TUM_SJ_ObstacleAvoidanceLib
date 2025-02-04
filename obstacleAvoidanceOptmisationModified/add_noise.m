function noisy_joint_vector = add_noise(joint_vector,noise_min,noise_max)


% Define the range of the uniform noise
% noise_min = -0.005;
% noise_max = 0.005;

% Generate uniform noise with the same size as the joint vector
noise = noise_min + (noise_max - noise_min) * rand(size(joint_vector));

% Add the noise to the original joint vector
noisy_joint_vector = joint_vector + noise;
end