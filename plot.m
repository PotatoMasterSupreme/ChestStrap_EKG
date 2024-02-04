% Path to the CSV file
csv_file_path = 'heart_rate_data.csv';

% Load the data
data = readtable(csv_file_path, 'ReadVariableNames', false);
heart_rate = data.Var1; % Assumes the heart rate data is in the first column

% Plot the data
figure;
plot(heart_rate, 'b', 'DisplayName', 'Heart Rate'); % 'b' stands for blue color
title('Heart Rate Data');
xlabel('Sample Number');
ylabel('Heart Rate Value');
legend;
grid on;
