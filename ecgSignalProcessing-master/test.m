
    % Load EKG data from a CSV file
    data1 = readmatrix("Recording 2 (250hz).csv");
    % Assuming your signal is stored in the variable 'originalSignal'
% and your original sampling rate is stored in 'Fs_original'

    Fs_original = 250; % Original sampling rate
    Fs_new = 250; % New desired sampling rate
    
    % Calculate the decimation factor
    decimationFactor = Fs_original / Fs_new;
    
    % Directly downsample the signal 
    data = downsample(data1(5:end, 2), decimationFactor);

% 'downsampledSignal' is now your signal sampled at 250 Hz

    ekgSignal = data; % Assuming the EKG signal is in the first column
    % Remove baseline wander
    ekgFiltered = removeBaselineWander(ekgSignal, 250); % Example: 250 Hz sampling rate
    
    % Remove unwanted frequencies (e.g., noise)
    ekgFiltered = removeUnwantedFrequencies(ekgFiltered, 250); % Example: 250 Hz sampling rate
    
    % Assess signal quality
    [rmseValue, snrValue] = assessFiltrationQuality(ekgFiltered, referenceSignalPath);
    
    fprintf('RMSE with reference signal: %f\n', rmseValue);
    fprintf('SNR with reference signal: %f\n', snrValue);
    

    fprintf('SNR before filtration: %f\n', snrBefore);
    fprintf('SNR after filtration: %f\n', snrAfter);
    
    % Plot the final signal and the raw signal
    figure;
    subplot(2, 1, 1);
    plot(ekgSignal);
    title('Raw EKG Signal');
    
    subplot(2, 1, 2);
    plot(ekgFiltered);
    title('Filtered EKG Signal');
   
function filteredSignal = removeBaselineWander(signal, Fs)
    % Remove baseline wander using a highpass filter
    % Design a highpass filter to remove baseline wander (e.g., <0.5 Hz)
    hpFilt = designfilt('highpassiir', 'FilterOrder', 5, 'HalfPowerFrequency', 1, 'SampleRate', Fs, 'DesignMethod', 'butter');
    filteredSignal = filtfilt(hpFilt, signal);
end

function ekgFiltered = removeUnwantedFrequencies(signal, Fs)
    % Remove unwanted frequencies using a bandpass filter
    % Example: Keep frequencies between 1 Hz and 40 Hz
    % Adjust the frequencies here to use it as a low-pass filter
    cutoff_freq = 40; % Adjust this cutoff frequency as needed
    lpFilt = designfilt('lowpassiir', ...
                        'FilterOrder', 5, ...
                        'HalfPowerFrequency', cutoff_freq, ...
                        'SampleRate', Fs, ...
                        'DesignMethod', 'butter');
    ekgFiltered = filtfilt(lpFilt, signal);
end

function [rmseValue,snrValue]= assessFiltrationQuality(mySignalPath, referenceSignalPath)
    % Load your filtered EKG signal
    mySignal = load(mySignalPath);
    
    % Load the reference EKG signal
    referenceSignal = load(referenceSignalPath);
    
    % Ensure both signals are the same length for comparison
    minLen = min(length(mySignal), length(referenceSignal));
    mySignal = mySignal(1:minLen);
    referenceSignal = referenceSignal(1:minLen);

    % Normalize both signals (optional, but often helpful)
    mySignal = (mySignal - mean(mySignal)) / std(mySignal);
    referenceSignal = (referenceSignal - mean(referenceSignal)) / std(referenceSignal);

    % Calculate SNR
    snrValue = snr(referenceSignal, referenceSignal - mySignal);
    
    % Calculate RMSE
    rmseValue = sqrt(mean((mySignal - referenceSignal).^2));
end
