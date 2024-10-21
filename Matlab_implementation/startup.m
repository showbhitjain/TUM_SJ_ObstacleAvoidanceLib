clear all;
clear, clc, close all;
disp('Starting Robot Manipulator Trajectory Follwoing Example...')

% Set up the MATLAB search path
rootDir = fileparts(which(mfilename));

addpath(genpath(pwd));
% Set the code generation and cache folders to a work folder
% If the folder does not exist, create it
if ~isfolder('work')
    mkdir('work');
end
Simulink.fileGenControl('set','CacheFolder','work','CodeGenFolder','work');


