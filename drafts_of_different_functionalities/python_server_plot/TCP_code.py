#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Feb  4 15:30:47 2024

@author: saifnoreldeen
"""

import socket
import struct

def start_tcp_server():
    # Create a TCP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Bind the socket to the port
    server_address = ('0.0.0.0', 5001)
    print(f"Starting TCP server on {server_address[0]} port {server_address[1]}")
    sock.bind(server_address)

    # Listen for incoming connections
    sock.listen(1)

    file_path = '/Users/saifnoreldeen/Documents/Arduino/code/DataFile'

    while True:
        print("\nWaiting for a connection...")
        connection, client_address = sock.accept()

        try:
            print(f"Connection from {client_address}")

            # If you prefer to store the data in binary format, use 'wb' mode
            # If you want to convert the binary data to text (e.g., for readability), use 'w' mode
            with open(file_path, 'w') as file:  # Change to 'wb' for binary file
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
                            
                            # For binary file, write the bytes directly
                            # file.write(data[i*4:(i+1)*4])
                            
                            # For text file, convert float to string and write
                            file.write(f"{float_value}\n")
                        
                        # Optionally, send a response back to the sender
                        connection.sendall(b"Data received\n")
                    else:
                        print("No more data from", client_address)
                        break
        except ConnectionResetError:
            print("Connection was reset by the client.")
        finally:
            # Clean up the connection
            connection.close()

if __name__ == '__main__':
    start_tcp_server()
