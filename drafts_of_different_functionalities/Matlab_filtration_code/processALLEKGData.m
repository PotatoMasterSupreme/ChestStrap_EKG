function processALLEKGData()
    % Let the user select a directory
    folderPath = uigetdir;
    referenceSignal = readmatrix('reference.csv');
    if folderPath == 0
        fprintf('No folder selected, operation cancelled.\n');
        return;
    end
    
    % Find all CSV files in the selected directory
    csvFiles = dir(fullfile(folderPath, '*.csv'));
    
    % Loop through each file and process it
    for i = 1:length(csvFiles)
        originalCsvFilePath = fullfile(folderPath, csvFiles(i).name);
        fprintf('Processing %s\n', originalCsvFilePath);
        
        % Generate the new filename for the filtered data
        [path, name, ~] = fileparts(originalCsvFilePath);
        filteredCsvFilePath = fullfile(path, [name 'filtered.csv']);
        
        % Process the EKG data
        [ekgFiltered, snrBefore, snrAfter, rmseBefore, rmseAfter] = processEKGData(originalCsvFilePath, referenceSignal, 250, 250);
        
        % Save the filtered signal
        % writematrix(ekgFiltered, filteredCsvFilePath);
        % fprintf('Filtered EKG data saved to: %s\n', filteredCsvFilePath);
        
        % Print out signal quality metrics
        fprintf('RMSE before: %f\n', rmseBefore);
        fprintf('RMSE After: %f\n',rmseAfter);
        fprintf('SNR before filtration: %f\n', snrBefore);
        fprintf('SNR after filtration: %f\n', snrAfter);
    end
end

function [ekgFiltered, snrBefore, snrAfter, rmseBefore, rmseAfter] = processEKGData(csvFilePath, referenceSignal, Fs_original, Fs_new)
    % Load EKG data from a CSV file
    data = readmatrix(csvFilePath);
    if width(data) == 2
    % Assume EKG signal starts from row 5, column 2 based on your data structure
    ekgSignal = data(5:end, 2);
    else
    ekgSignal = data;
    % Apply resampling if necessary (Fs_original ~= Fs_new)
    end
    if Fs_original ~= Fs_new
        decimationFactor = Fs_original / Fs_new;
        ekgSignal = downsample(ekgSignal, decimationFactor);
    end
    
    % Initial signal quality assessment before filtering
    [rmseBefore, snrBefore] = assessSignalQuality(ekgSignal, referenceSignal);
    
    % Filter operations
    ekgFiltered = removeBaselineWander(ekgSignal, Fs_new);
    ekgFiltered = removeUnwantedFrequencies(ekgFiltered, Fs_new);
    
    % Signal quality assessment after filtering
    [rmseAfter, snrAfter] = assessSignalQuality(ekgFiltered, referenceSignal);
    
    % Optional: Plot signals for visual comparison
    figure;
    subplot(2, 1, 1);
    plot(ekgSignal);
    title('Raw EKG Signal');
    subplot(2, 1, 2);
    plot(ekgFiltered);
    title('Filtered EKG Signal');
end


function [rmse, snr] = assessSignalQuality(signal, referenceSignal)
    % Ensure both signals are of the same length for comparison
    minLen = min(length(signal), length(referenceSignal));
    signal = signal(1:minLen);
    referenceSignal = referenceSignal(1:minLen);
    
    % Calculate RMSE
    rmse = sqrt(mean((signal - referenceSignal).^2));
    
    % Calculate signal and noise power for SNR
    signalPower = mean(signal.^2);
    noisePower = mean((signal - referenceSignal).^2);
    snr = abs(10 * log10(signalPower / noisePower));
end




function filteredSignal = removeBaselineWander(signal, Fs)
    hpFilt = designfilt('highpassiir', 'FilterOrder', 5, 'HalfPowerFrequency', 0.6, 'SampleRate', Fs, 'DesignMethod', 'butter');
    filteredSignal = filtfilt(hpFilt, signal);
end

function ekgFiltered = removeUnwantedFrequencies(signal, Fs)
    cutoff_freq = 35;
    lpFilt = designfilt('lowpassiir', 'FilterOrder', 5, 'HalfPowerFrequency', cutoff_freq, 'SampleRate', Fs, 'DesignMethod', 'butter');
    ekgFiltered = filtfilt(lpFilt, signal);
end

