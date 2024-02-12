#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Feb  4 09:07:29 2024

@author: saifnoreldeen
"""

import numpy as np
import matplotlib.pyplot as plt

def generate_pulse_waveform(duration=60, sample_rate=250):
    """
    Generate a pulse waveform similar to the Arduino sketch.
    
    :param duration: Duration of the signal in seconds.
    :param sample_rate: Sampling rate in Hz.
    :return: Tuple (times, waveform) where 'times' is a numpy array of time stamps,
             and 'waveform' is the generated pulse waveform.
    """
    t = np.linspace(0, duration, num=duration*sample_rate, endpoint=False)
    pulse_frequency = 1.0  # Pulse frequency in Hz
    waveform = np.sin(2 * np.pi * pulse_frequency * t) * np.exp(-np.mod(t, 1/pulse_frequency) * 4)
    return t, waveform

def plot_waveform(t, waveform):
    """
    Plot the generated pulse waveform and its first 5 seconds.
    
    :param t: Time stamps for the waveform.
    :param waveform: The generated pulse waveform.
    """
    # Create a subplot with 1 row and 2 columns
    fig, axs = plt.subplots(2, 1)
    
    # Plot the entire waveform on the first subplot
    axs[0].plot(t, waveform, label='Pulse Waveform', color='blue')
    axs[0].set_title('Generated Pulse Waveform Over 1 Minute')
    axs[0].set_xlabel('Time (seconds)')
    axs[0].set_ylabel('Waveform Amplitude')
    axs[0].grid(True)
    axs[0].legend()
    
    # Find the index where t=5 seconds to plot the first 5 seconds on the second subplot
    idx_5_sec = np.where(t <= 5)[0][-1]
    axs[1].plot(t[:idx_5_sec+1], waveform[:idx_5_sec+1], label='First 5 Seconds', color='red')
    axs[1].set_title('Generated Pulse Waveform First 5 Seconds')
    axs[1].set_xlabel('Time (seconds)')
    axs[1].set_ylabel('Waveform Amplitude')
    axs[1].grid(True)
    axs[1].legend()
    
    # Show the plot
    plt.show()

if __name__ == "__main__":
    t, waveform = generate_pulse_waveform(duration=60, sample_rate=250)
    plot_waveform(t, waveform)
