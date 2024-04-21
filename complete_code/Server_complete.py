#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Apr  2 23:08:07 2024

@author: saifnoreldeen
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import socket
import struct
from datetime import datetime



def plot_data(buffer):
    """
    Plot the data from the buffer.
    Assumes the buffer is a list of float values.
    """
    data = pd.DataFrame(buffer, columns=['EKG Value'])
    
    sample_rate = 250  # 250 samples per second

    # Time vector in seconds
    time_vector = np.arange(0, len(data) / sample_rate, 1 / sample_rate)

    # Assuming you want to plot 5 seconds of EKG data
    five_seconds = 5 * sample_rate  # Number of samples in 5 seconds

    # Create a figure and a set of subplots
    fig, axs = plt.subplots(2, 1, figsize=(10, 8))

    # Adjusted plotting to fix the Locator.MAXTICKS error:
    # Plot the entire dataset on the first subplot
    axs[0].plot(time_vector, data['EKG Value'])
    axs[0].set_title('Entire EKG Data Plot')
    axs[0].set_xlabel('Time (s)')
    axs[0].set_ylabel('EKG Value')

    # Configure major ticks to reasonable intervals
    axs[0].xaxis.set_major_locator(plt.MaxNLocator(10))  # Set maximum number of major ticks
    axs[0].yaxis.set_major_locator(plt.MaxNLocator(10))  # Adjust based on your data range
    axs[0].grid(True)

    # Plot the first 5 seconds of data on the second subplot, if available
    if len(data) > five_seconds:
        axs[1].plot(time_vector[:five_seconds], data['EKG Value'][:five_seconds], color='red')
        axs[1].set_title('First 5 Seconds of EKG Data Plot')
        axs[1].set_xlabel('Time (s)')
        axs[1].set_ylabel('EKG Value')
        
        # Configure major ticks to reasonable intervals
        axs[1].xaxis.set_major_locator(plt.MaxNLocator(10))  # Set maximum number of major ticks
        axs[1].yaxis.set_major_locator(plt.MaxNLocator(10))  # Adjust based on your data range
        axs[1].grid(True)
    else:
        print("The dataset contains less than 5 seconds of data.")

    # Adjust layout to prevent overlap and show the plot
    plt.tight_layout()
    plt.show()

def start_tcp_server():
    # Create a TCP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the port
    server_address = ('0.0.0.0', 5001)
    print(f"Starting TCP server on {server_address[0]} port {server_address[1]}")
    sock.bind(server_address)

    # Listen for incoming connections
    sock.listen(1)

    base_file_path = '/Users/saifnoreldeen/Documents/Arduino/code/DataFileTest'

    while True:
        print("\nWaiting for a connection...")
        connection, client_address = sock.accept()

        try:
            print(f"Connection from {client_address}")
            # Buffer to store received data for plotting
            
            plot_buffer = []
            # Generate a unique file path for each connection
            timestamp = datetime.now().strftime("%Y%m%d%H%M%S")
            unique_file_path = f"{base_file_path}_{timestamp}.txt"  # or .bin for binary files

            # Open the file in 'w' mode for text, 'wb' for binary
            with open(unique_file_path, 'w') as file:  
                while True:
                    data = connection.recv(4096)  # Buffer size is 4096 bytes
                    if data:
                        print(f"Received {len(data)} bytes from {client_address}")
                        
                        # Process received bytes in chunks of 4 (size of a float)
                        floats_received = len(data) // 4
                        for i in range(floats_received):
                            # Unpack the bytes to float
                            float_value, = struct.unpack('<f', data[i*4:(i+1)*4])
                            print(float_value)
                            
                            plot_buffer.append(float_value)
                            
                            # Write the float value to file
                            file.write(f"{float_value}\n")
                        # Plot the received data
                        
                        
                    else:
                        print("No more data from", client_address)
                        plot_data(plot_buffer)
                        plot_buffer.clear()  # Clear the buffer after plotting
                        break
        except ConnectionResetError:
            print("Connection was reset by the client.")
        finally:
            # Clean up the connection
            connection.close()

if __name__ == '__main__':
    start_tcp_server()
