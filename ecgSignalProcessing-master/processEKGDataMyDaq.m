function processEKGDataMyDaq(csvFilePath)
    % Load EKG data from a CSV file
    data1 = readmatrix("Chest-Strap_1.csv");
    % Assuming your signal is stored in the variable 'originalSignal'
    % and your original sampling rate is stored in 'Fs_original'

    Fs_original = 100000; % Original sampling rate
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
    
    % Analyze SNR before and after filtration
    snrBefore = calculateSNR(ekgSignal);
    snrAfter = calculateSNR(ekgFiltered);
    
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
end

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

function snrValue = calculateSNR(signal)
    % Placeholder for SNR calculation
    % This function should be customized based on how you define SNR for your application.
    % As an example, assuming the noise is the difference between the signal and its low-pass filtered version
    lpFilt = designfilt('lowpassiir', 'FilterOrder', 5, 'HalfPowerFrequency', 100, 'SampleRate', 250, 'DesignMethod', 'butter');
    signalClean = filtfilt(lpFilt, signal);
    noise = signal - signalClean;
    snrValue = snr(signalClean, noise);
end

