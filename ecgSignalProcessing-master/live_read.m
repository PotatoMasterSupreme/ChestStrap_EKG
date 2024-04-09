% Define UDP port and create UDP object
udpPort = 5001;
u = udpport("LocalPort", udpPort);


% Initialize figure for plotting

fig = figure;
ax = axes(fig);
ylim(ax, [-1, 1]); % Adjust based on your data range
lineHandle = animatedline(ax, 'Color', 'r');
xlabel(ax, 'Time');
ylabel(ax, 'Data');

% Ensure the rest of your setup code here, especially for the UDP object

% Sampling information
sampleRate = 250; % Adjust this based on your actual sampling rate
plotWindowSeconds = 5; % Window length in seconds
numSamplesInWindow = sampleRate * plotWindowSeconds;


% Preallocate buffer for efficiency
dataBuffer = nan(numSamplesInWindow, 1);
timeBuffer = nan(numSamplesInWindow, 1);

% Modify the loop to run indefinitely
while ishandle(fig) % Loop until the figure is closed
    if u.NumBytesAvailable > 0
        % Read a single float value from the UDP port
        data = read(u, 1, "single");
        
        % Update buffers with the new data point
        dataBuffer = [dataBuffer(2:end); data];
        
        % Assuming a continuous flow of data, update the time buffer
        if isnan(timeBuffer(end))
            currentTime = 0; % Initialize at the start
        else
            currentTime = timeBuffer(end) + (1/sampleRate);
        end
        timeBuffer = [timeBuffer(2:end); currentTime];
        
        % Clear the existing plot
        clearpoints(lineHandle);
        
        % Redraw the plot with the updated data
        validIndices = ~isnan(timeBuffer);
        addpoints(lineHandle, timeBuffer(validIndices), dataBuffer(validIndices));
        
        % Adjust the x-axis limits to create a rolling window effect
        xlim(ax, [currentTime - plotWindowSeconds, currentTime]);
        
        drawnow limitrate;
    end
    
    % Pause briefly to reduce CPU usage
    pause(0.001); % Adjust as needed to balance responsiveness and CPU usage
end

% Clean up after the figure is closed
clear u
