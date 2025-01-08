# Attendance System Project

Welcome to the **Attendance System Project**, a microcontroller-based intelligent system designed to streamline attendance management and enhance classroom organization.

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
   - **Submit Student Code**:
     - Students can enter their unique 8-digit ID via the keypad.
     - The system validates the format. Errors, such as invalid length or format, are displayed on the LCD, and the buzzer is activated for feedback.
   - **Exit**:
     - This option allows users to return to the main menu.

2. **Student Management**:
   - **Search Students**:
      - Enter a student ID to check if the student is present or absent.
   - **Delete Students**:
      - Enter a student ID to delete it from the records.
   - **Exit**:
     - This option allows users to return to the main menu.

3. **View Present Students**:
   - Displays the total number of present students.
   - Lists the IDs of present students on the LCD, updating dynamically.

4. **Temperature Monitoring**:
   - Displays real-time environmental temperature using the LM35 sensor.
   - Temperature values are converted from ADC readings and updated on the LCD.

5. **Retrieve Student Data**:
   - Fetch stored student data from EEPROM.
   - Transfer data via USART to external systems. Success or error messages are displayed on the LCD.

6. **Traffic Monitoring**:
   - Monitors real-time classroom traffic using an ultrasonic sensor.
   - Traffic data is displayed on the LCD for better classroom management.

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

1. Clone this repository:

   ```bash
   git clone https://github.com/your-username/attendance-system.git
2. **Open the Proteus Simulation**:

   - Navigate to the `simulation` folder in the repository.
   - Open the `.pdsprj` file in **Proteus** to view and simulate the project.

3. **Load the Hex File**:

   - Inside the Proteus simulation, double-click on the ATmega32 microcontroller.
   - In the configuration window, browse to the `hex` folder and select the provided `.hex` file.
   - Apply the settings and close the configuration window.

4. **Compile and Upload Code (Optional)**:

   - If you want to modify the code, open the project in **Atmel Studio** from the `code` folder.
   - Compile the project to generate a new `.hex` file.
   - Replace the existing `.hex` file in the Proteus simulation with the newly generated one.

5. **Run the Simulation**:

   - Press the play button in **Proteus** to run the simulation.
   - Interact with the system using the keypad and observe the LCD, buzzer, and other components.


