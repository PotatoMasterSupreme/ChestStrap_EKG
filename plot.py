#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Feb  4 09:04:27 2024

@author: saifnoreldeen
"""

import pandas as pd
import matplotlib.pyplot as plt

# Path to the CSV file
csv_file_path = 'heart_rate_data.csv'

def plot_heart_rate_data(csv_file_path):
    # Load the data
    data = pd.read_csv(csv_file_path, header=None, names=['Heart Rate'])

    # Plot the data
    plt.figure(figsize=(10, 6))
    plt.plot(data['Heart Rate'], label='Heart Rate', color='blue')
    plt.title('Heart Rate Data')
    plt.xlabel('Sample Number')
    plt.ylabel('Heart Rate Value')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    plot_heart_rate_data(csv_file_path)
