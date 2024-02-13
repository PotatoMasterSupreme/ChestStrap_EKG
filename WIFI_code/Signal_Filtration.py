#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Feb 13 00:00:15 2024

@author: saifnoreldeen
"""

import pandas as pd
import numpy as np
from scipy.signal import butter, lfilter
import matplotlib.pyplot as plt

# Functions for filtering
def butter_bandpass(lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    return b, a

def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

# Read the EKG data
file_path = '/Users/saifnoreldeen/Documents/Arduino/code/DataFile'
data = pd.read_csv(file_path, header=None, names=['Value'])

# Sampling rate and filter parameters
fs = 250.0  # Sampling rate, Hz
lowcut = 0.5  # Low cutoff frequency, Hz
highcut = 50.0  # High cutoff frequency, Hz

# Apply the bandpass filter
filtered_data = butter_bandpass_filter(data['Value'], lowcut, highcut, fs, order=8)

# Plotting the original and filtered EKG data
fig, axs = plt.subplots(2, 2, figsize=(15, 10))  # 2 rows, 2 columns

# Full dataset plots
axs[0, 0].plot(data['Value'], label='Original EKG Signal', alpha=0.5)
axs[0, 0].set_title('Original EKG Signal')
axs[0, 0].set_xlabel('Sample Number')
axs[0, 0].set_ylabel('Amplitude')
axs[0, 0].legend()

axs[0, 1].plot(filtered_data, label='Filtered EKG Signal', color='red')
axs[0, 1].set_title('Filtered EKG Signal')
axs[0, 1].set_xlabel('Sample Number')
axs[0, 1].set_ylabel('Amplitude')
axs[0, 1].legend()

# First 5 seconds plots
samples_5_sec = int(5 * fs)  # Number of samples in the first 5 seconds
axs[1, 0].plot(data['Value'][:samples_5_sec], label='First 5 Seconds Original', alpha=0.5)
axs[1, 0].set_title('First 5 Seconds Original Signal')
axs[1, 0].set_xlabel('Sample Number')
axs[1, 0].set_ylabel('Amplitude')
axs[1, 0].legend()

axs[1, 1].plot(filtered_data[:samples_5_sec], label='First 5 Seconds Filtered', color='red')
axs[1, 1].set_title('First 5 Seconds Filtered Signal')
axs[1, 1].set_xlabel('Sample Number')
axs[1, 1].set_ylabel('Amplitude')
axs[1, 1].legend()

plt.tight_layout()
plt.show()

