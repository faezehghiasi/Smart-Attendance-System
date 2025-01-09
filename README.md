# Attendance System Project

Welcome to the **Attendance System Project**, a microcontroller-based intelligent system designed to streamline attendance management and enhance classroom organization.

---
## Table of Contents

1. [Project Overview](#project-overview)
2. [Features](#features)
   - [Attendance Initialization](#attendance-initialization)
   - [Student Management](#student-management)
   - [View Present Students](#view-present-students)
   - [Temperature Monitoring](#temperature-monitoring)
   - [Retrieve Student Data](#retrieve-student-data)
   - [Traffic Monitoring](#traffic-monitoring)
3. [Project Demo](#project-demo)
4. [Components Used](#components-used)
5. [Installation and Usage](#installation-and-usage)

---
## Project Overview

This project leverages the features of the AVR microcontroller (ATmega32) to create an efficient system for:
- **Tracking student attendance** during classes.
- **Monitoring traffic and temperature** in real-time.
- **Providing user-friendly interactions** through a keypad and LCD.
- **Ensuring data persistence** with EEPROM storage and USART communication.

Built using the **C programming language**, this project offers a robust and scalable solution for managing attendance and environmental monitoring in educational settings.

---

## Features

### Main Functionalities

1. **Attendance Initialization**:
   - The system prompts the user to set a specific time limit for attendance registration. Students must register their attendance before the timer expires. Once the time is up, attendance registration will automatically close, and the system will display a summary of the registered students.
   - **Submit Student Code**:
     - Students can enter their unique 8-digit ID via the keypad.
     - The system validates the ID format. If the input is invalid (e.g., incorrect length or format), an error message is displayed on the LCD, and the buzzer provides audio feedback.
   - **Exit**:
     - Allows users to return to the main menu without making any changes.

3. **Student Management**:
   - **Search Students**:
      - Enter a student ID to check if the student is present or absent.
   - **Delete Students**:
      - Enter a student ID to delete it from the records.
   - **Exit**:
     - This option allows users to return to the main menu.

4. **View Present Students**:
   - Displays the total number of present students.
   - Lists the IDs of present students on the LCD, updating dynamically.

5. **Temperature Monitoring**:
   - Displays real-time environmental temperature using the LM35 sensor.
   - Temperature values are converted from ADC readings and updated on the LCD.

6. **Retrieve Student Data**:
   - Fetch stored student data from EEPROM.
   - Transfer data via USART to external systems. Success or error messages are displayed on the LCD.

7. **Traffic Monitoring**:
   - Monitors real-time classroom traffic using an ultrasonic sensor.
   - Traffic data is displayed on the LCD for better classroom management.
---

## Project Demo

https://github.com/user-attachments/assets/b01ed524-7279-480d-a86e-2758669c1cc5


---

## Components Used

- **Keypad**: For entering student IDs and navigating the menu.
- **LCD**: For displaying system states and user instructions.
- **USART**: For transferring data to external memory or systems.
- **Buzzer**: For audio feedback and error notifications.
- **Ultrasonic Sensor**: For monitoring traffic.
- **Temperature Sensor**: For environmental temperature tracking.

---

## Installation and Usage

1. **Clone this repository**:

   ```bash
   git clone https://github.com/your-username/attendance-system.git
2. **Open the Proteus Simulation**:

   - Navigate to the `simulation` folder in the repository.
   - Open the `.pdsprj` file in **Proteus** to view and simulate the project.

3. **Compile the Source Code**:

   - Navigate to the `Smart-Attendance-System/src` folder in the repository.
   - Open the source files in **Atmel Studio** or your preferred IDE.
   - Compile the project to generate a new `.hex` file.

4. **Load the Hex File**:

   - Inside the Proteus simulation, double-click on the ATmega32 microcontroller.
   - In the configuration window, browse to the location of the newly compiled `.hex` file.
   - Apply the settings and close the configuration window.

5. **Run the Simulation**:

   - Press the play button in **Proteus** to run the simulation.
   - Interact with the system using the keypad and observe the LCD, buzzer, and other components.



