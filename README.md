# Simple Intrusion Detection System

The "Simple Intrusion Detection System" is a lightweight, efficient program designed to monitor and detect unauthorized entry or activity within a specified area using video analysis. Built with OpenCV in C++, this project leverages computer vision techniques to analyze live video streams, identify potential intrusions, and respond appropriately by recording the event and alerting the system operator.

## Features

- **Real-Time Video Processing**: Utilizes live feed from a connected camera to continuously monitor for movement or activity that could indicate an intrusion.
- **Motion Detection**: Employs optical flow and kinetic energy calculations to detect moving objects within the camera's field of view.
- **Event Recording**: Automatically records video segments when potential intrusions are detected, saving files with timestamps for easy review.
- **Graceful Shutdown**: Safely terminates the program and releases resources when interrupted (e.g., via `Ctrl+C`), ensuring all ongoing recordings are properly saved.
- **Headless Operation**: Designed to run on headless systems (no GUI), making it ideal for server environments or dedicated monitoring devices.

## Installation

### Prerequisites

- Ubuntu operating system (e.g., Ubuntu 20.04)
- C++ compiler
- OpenCV library (version 4.x)

### Steps

1. **Install OpenCV and g++**

   Open a terminal and run the following commands to install the necessary packages:
   
   ```sh
   sudo apt update
   sudo apt install -y build-essential libopencv-dev
   ```

2. **Clone the Repository**

   Clone the repository to your local machine:
   
   ```sh
   git clone https://github.com/Coldmooon/Simple-Intrusion-Detection-System.git
   cd Simple-Intrusion-Detection-System
   ```

3. **Compile the Source Code**

   Compile the source code using g++ and link it with the OpenCV library:
   
   ```sh
   g++ main.cpp -o SimpleIDS `pkg-config --cflags --libs opencv4`
   ```

   This will create an executable named `SimpleIDS`.

## Usage

1. **Run the Program**

   Execute the binary to start the intrusion detection system:
   
   ```sh
   ./SimpleIDS
   ```

   Ensure your system has a connected camera that the program can access.

2. **Terminating the Program**

   To safely terminate the program and ensure all resources are released, use `Ctrl+C` in the terminal. The program is designed to handle this signal gracefully, saving any ongoing recordings before exiting.

3. **Viewing Recorded Videos**

   By default, the program saves recorded video segments in the current working directory, with filenames containing the timestamp of when the recording started. Navigate to the directory where you ran the program to view the recordings.

### Notes

- The program has been tested on Ubuntu 20.04 with OpenCV 4.2.0. Compatibility with other versions or operating systems may vary.
- If you encounter permission issues accessing the camera, ensure your user is in the `video` group, or try running the program with `sudo`.
