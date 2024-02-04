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
    Plot the generated pulse waveform.
    
    :param t: Time stamps for the waveform.
    :param waveform: The generated pulse waveform.
    """
    plt.figure(figsize=(10, 6))
    plt.plot(t, waveform, label='Pulse Waveform', color='blue')
    plt.title('Generated Pulse Waveform Over 1 Minute')
    plt.xlabel('Time (seconds)')
    plt.ylabel('Waveform Amplitude')
    plt.grid(True)
    plt.legend()
    plt.show()

if __name__ == "__main__":
    t, waveform = generate_pulse_waveform(duration=60, sample_rate=250)
    plot_waveform(t, waveform)
