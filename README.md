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
   - Enter "Attendance Ready" mode for seamless student attendance tracking.

2. **Submit Student Code**:
   - Students enter their unique 8-digit code using the keypad.
   - The system validates the format and notifies errors via the buzzer and LCD.

3. **Exit**:
   - Return to the main menu for other operations.

4. **View Present Students**:
   - Display the count and names of present students in real-time.

5. **Student Management**:
   - Add or delete student records.
   - Perform searches to verify student attendance status.

6. **Temperature Monitoring**:
   - Monitor and display the classroom’s temperature using the LM35 sensor.

7. **Traffic Monitoring**:
   - Real-time traffic analysis using an ultrasonic sensor for classroom congestion monitoring.

8. **Retrieve Student Data**:
   - Fetch student data from EEPROM or transfer via USART to an external system.

### Advanced Options (Extra Credit)
- Support for **RFID cards** to automate student identification.
- Transition to **GLCD display** for enhanced visuals.
- **Time constraints** for attendance submission (e.g., 15-minute window).
- Prevention of duplicate attendance entries with warnings.
- Automatic storage of attendance **time and date** for precise reporting.

---

## Components Used

- **Keypad**: For entering student IDs and navigating the menu.
- **LCD**: For displaying system states and user instructions.
- **USART**: For transferring data to external memory or systems.
- **Buzzer**: For audio feedback and error notifications.
- **Ultrasonic Sensor**: For monitoring traffic.
- **Temperature Sensor**: For environmental temperature tracking.

---

## How It Works

1. **Attendance Submission**:
   - Students enter their 8-digit ID using the keypad.
   - Input is validated, and feedback is provided via the LCD and buzzer.

2. **Data Persistence**:
   - Student IDs are securely stored in EEPROM to ensure data is retained after power loss.

3. **Environmental Monitoring**:
   - Temperature and traffic data are continuously monitored and displayed.

4. **Error Handling**:
   - Invalid inputs trigger error messages and alerts.
   - Duplicate attendance entries are prevented with appropriate warnings.

---

## System Architecture

### Hardware Configuration

- **Keypad**: 4×3 matrix connected to PORTA.
- **LCD**: 8-bit mode connected to PORTC and control pins on PORTD.
- **Ultrasonic Sensor**: Trigger and Echo pins connected to PORTB.
- **Temperature Sensor**: Analog output connected to ADC7 (PORTA.7).
- **Buzzer**: Output on PORTB.2.
- **USART**: TXD connected to PORTD.1.

### Software Modules

- **EEPROM Management**:
  - Functions to add, delete, and search student IDs.
- **Menu Navigation**:
  - Intuitive menu system for smooth user interaction.
- **Interrupt Handling**:
  - INT0 interrupt for efficient keypad input detection.
- **Data Conversion**:
  - Analog-to-digital conversion for temperature monitoring.

---

## Installation and Usage

1. Clone this repository:

   ```bash
   git clone https://github.com/your-username/attendance-system.git
