#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Feb  4 08:53:53 2024

@author: saifnoreldeen
"""

import serial
import time

# Configuration
serial_port = '/dev/cu.usbmodem1401'  # Update with the actual serial port
baud_rate = 9600  # Match the baud rate to the Arduino configuration
output_file_path = 'heart_rate_data.csv'  # File to store the heart rate data

def send_activation_command_and_save_data():
    with serial.Serial(serial_port, baud_rate) as ser, open(output_file_path, 'w') as file:
        print("Sending activation command...")
        ser.write(b'start\n')  # Send the command to start data collection
        print("Command sent. Collecting data...")
        time.sleep(1)  # Wait a bit for Arduino to start sending data

        # Collect data for 1 minute
        start_time = time.time()
        while time.time() - start_time < 60:  # Adjust duration as needed
            if ser.in_waiting:
                data_line = ser.readline().decode('utf-8').rstrip()
                print(data_line)  # Print to console for verification
                file.write(data_line + '\n')  # Write to CSV file

        print(f"Data collection complete. Data saved to {output_file_path}")

if __name__ == "__main__":
    send_activation_command_and_save_data()

