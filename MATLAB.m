clc; clear;

max_iterations = 100;
output_file = 'output_positions.txt';
sfla_exe_path = 'C:\Users\phate\OneDrive\Desktop\UNI\HamTarahi\q003\x64\Debug\q003.exe';

% Check if SystemC executable exists before running
if exist(sfla_exe_path, 'file') ~= 2
    error('Error: sfla_systemc.exe not found at %s', sfla_exe_path);
end

for iter = 1:max_iterations
    fprintf('Iteration: %d\n', iter);

    % Run SystemC simulation
    system(['"', sfla_exe_path, '"']);

    % Wait for output file to be generated
    while exist(output_file, 'file') ~= 2
        pause(0.1);
    end

    % Read updated positions
    updated_positions = dlmread(output_file);
    disp('Updated Positions from SystemC:');
    disp(updated_positions);
    
    if iter >= max_iterations
        break;
    end
end

disp('SFLA Algorithm Completed.');
