
# EKG Monitoring System

This repository contains the source code for a complete EKG monitoring system which includes device firmware, data transmission over TCP and UDP, and EKG signal processing. This project is designed to capture EKG signals, transmit them over the network, and process the signals for medical analysis.

## Project Structure

- `/complete_code`: Contains the source code that runs on the ESP32 device for capturing EKG data and sending it over TCP/UDP.
- `/Server_complete.py`: Python scripts for setting up a TCP server to receive and process EKG data.
- `/live_read.m`: MATLAB scripts for receiving EKG data over UDP and plotting it in real-time.
- `/EKGFiltering`: Contains MATLAB and Python scripts for filtering and processing EKG signals.
- `/drafts_of_different_functionalities`: Example scripts and usage scenarios.

## Features

- **Real-time EKG Data Capture**: Firmware for ESP32 to sample EKG data at 250 Hz.
- **Data Transmission**: Supports both TCP and UDP for flexible, real-time data streaming.
- **Signal Processing**: Includes algorithms for filtering and analyzing EKG signals to extract medical insights.
- **Visualization**: MATLAB scripts for plotting EKG data in real-time.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- ESP32 development board
- MATLAB (R2020b or later recommended for UDP functionality)
- Python 3.8 or later

### Installing

1. **Clone the repository:**
   ```bash
   git clone https://github.com/PotatoMasterSupreme/ChestStrap_EKG.git
   
Device Setup:
Flash the ESP32 with the firmware located in /DeviceCode.
Connect the EKG sensor as per the schematics provided in /Documentation.
Running the TCP Server:
Navigate to the /TCPServer directory.
Run the Python server:
bash
Copy code
python tcp_server.py
Running the MATLAB UDP Listener:
Open MATLAB and navigate to /UDPMatlab.
Run the script:
matlab
Copy code
udp_listener.m
Data Processing:
Review the /EKGFiltering directory for scripts on processing the data.
Usage

Detail how to use the system, include any relevant scripts or commands. If applicable, also provide examples of how to run the network servers, how to process data, and how to visualize it.

Contributing

Please read CONTRIBUTING.md for details on our code of conduct, and the process for submitting pull requests to us.

### Authors

Saif Noreldeen - Initial work - PotatoMasterSupreme

License

This project is licensed under the MIT License - see the LICENSE.md file for details.


Acknowledgments

Hat tip to anyone whose code was used
Dr. Hunter's filtration code was used in part to develop some of the filtration
etc

Contact

For major questions, issues, and collaborations, please contact your.email@example.com.

