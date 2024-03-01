#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Feb  4 22:22:58 2024

@author: saifnoreldeen
"""

import pandas as pd
import matplotlib.pyplot as plt

file_path = '/Users/saifnoreldeen/Documents/Arduino/code/DataFile'

# Directly read the data into a DataFrame
data = pd.read_csv(file_path, header=None, names=['Value'])

# Plotting the data
plt.figure(figsize=(10, 6))  # Set the figure size (optional)
plt.plot(data['Value'], marker='o', linestyle='-', markersize=2)  # Plot the data
plt.title('Data Plot')  # Title of the plot
plt.xlabel('Index')  # X-axis label
plt.ylabel('Value')  # Y-axis label
plt.grid(True)  # Show grid
plt.show()  # Display the plot
