% Open the .dat file
fileID = fopen('rec_1.dat', 'r');

% Check if file opening succeeded
if fileID == -1
    error('File open failed.');
end

% Read the data
% Modify 'int16' based on the actual data format and multiply by the number of channels if necessary
data = fread(fileID, 'int16');
Fs_original = 500; % Original sampling rate
Fs_new = 250; % New desired sampling rate
    
    % Calculate the decimation factor
 decimationFactor = Fs_original / Fs_new;
    
    % Directly downsample the signal 
    data = downsample(data, decimationFactor);
ekgSignal = data(:, 1); % Assuming the EKG signal is in the first column
    
    % Remove baseline wander
    ekgFiltered = removeBaselineWander(ekgSignal, 250); % Example: 250 Hz sampling rate
    
    % Remove unwanted frequencies (e.g., noise)
    ekgFiltered = removeUnwantedFrequencies(ekgFiltered, 250); % Example: 250 Hz sampling rate
    

    
    % Plot the final signal and the raw signal
    figure;
    subplot(2, 1, 1);
    plot(ekgSignal);
    title('Raw EKG Signal');
    
    subplot(2, 1, 2);
    plot(ekgFiltered);
    title('Filtered EKG Signal');
    writematrix(ekgFiltered, 'reference.csv');
fclose(fileID);


function filteredSignal = removeBaselineWander(signal, Fs)
    % Remove baseline wander using a highpass filter
    % Design a highpass filter to remove baseline wander (e.g., <0.5 Hz)
    hpFilt = designfilt('highpassiir', 'FilterOrder', 5, 'HalfPowerFrequency', .5, 'SampleRate', Fs, 'DesignMethod', 'butter');
    filteredSignal = filtfilt(hpFilt, signal);
end

function ekgFiltered = removeUnwantedFrequencies(signal, Fs)
    % Remove unwanted frequencies using a bandpass filter
    % Example: Keep frequencies between 1 Hz and 40 Hz
    % Adjust the frequencies here to use it as a low-pass filter
    cutoff_freq = 20; % Adjust this cutoff frequency as needed
    lpFilt = designfilt('lowpassiir', ...
                        'FilterOrder', 5, ...
                        'HalfPowerFrequency', cutoff_freq, ...
                        'SampleRate', Fs, ...
                        'DesignMethod', 'butter');
    ekgFiltered = filtfilt(lpFilt, signal);
end