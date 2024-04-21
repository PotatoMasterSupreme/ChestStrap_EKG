function processEKGData(csvFilePath)
    % Load EKG data from a CSV file
    data = readmatrix("DataFile");
    ekgSignal = data(:, 1); % Assuming the EKG signal is in the first column
    
    % Remove baseline wander
    ekgFiltered = removeBaselineWander(ekgSignal, 250); % Example: 250 Hz sampling rate
    
    % Remove unwanted frequencies (e.g., noise)
    ekgFiltered = removeUnwantedFrequencies(ekgFiltered, 250); % Example: 250 Hz sampling rate
    
    % Analyze SNR before and after filtration
    snrBefore = calculateSNR(ekgSignal, ekgSignal); % Before filtering, compare the signal to itself
    snrAfter = calculateSNR(ekgSignal, ekgFiltered); % After filtering, compare original to filtered

    
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

function snrValue = calculateSNR(originalSignal, filteredSignal)
    % Estimate noise as the difference between the original and filtered signals
    noise = originalSignal - filteredSignal;
    
    % Calculate the power of the filtered signal and the noise
    % Power is the square of the signal, averaged over its length
    signalPower = mean(filteredSignal.^2);
    noisePower = mean(noise.^2);
    
    % Calculate SNR in dB
    snrValue = 10 * log10(signalPower / noisePower);
end
