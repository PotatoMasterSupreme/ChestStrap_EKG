filename = 'data.bin'; % Adjust the path accordingly
fileID = fopen(filename, 'r'); % Open the file for reading
ekgData = fread(fileID, inf, 'single'); % Read an infinite number of 'single' precision floats
fclose(fileID); % Close the file

% Plot the data
plot(ekgData);
xlabel('Sample Index');
ylabel('EKG Reading');
title('EKG Data');
