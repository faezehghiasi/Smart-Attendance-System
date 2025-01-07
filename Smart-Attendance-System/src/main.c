#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "LCD.h"
#include "Keypad.h"
#include "globals.h"
#include "EEPROM.h"
#include "temperature_monitoring.h"
#include "USART.h"
#include "time_limiter.h"
#include"ULTRASONIC.h"
//**********************************************************************************************************
void display_menu(int page);
void Attendance_Ready_menu_page();
void Submit_Student_Code();
void BUZZER();
bool check_stu_id(char * stuId);
void Wrong_format_of_stu_ID();
unsigned long int get_student_id(void);
void invalid_key_pressed(void);
void return_to_main_menu(void);
bool handle_student_search(unsigned long int stuID);
void display_student_management_menu_page(int page);
void Student_Management(void);
void Retrieve_Student_Data();
void display_students();
void View_Present_Students();
void Attendance_Ready();
void Traffic_Monitoring();
uint32_t get_attendance_time();
//**********************************************************************************************************
int main(void) {
    LCD_init();
    keypad_init();
    init_interrupt();
    SET_NUM_OF_STUDENTS(0); // Initialize the number of students in EEPROM
    USART_init(MYUBRR);
    ADC_init();

    sei();

    while (1) {
        if (menu_needs_update) {
            LCD_clear();
            display_menu(current_page);
            menu_needs_update = 0;
        }
        while (key_pressed == 0);
        if (key_pressed) {
            LCD_clear();
            if (key_pressed == '1') {
               Attendance_Ready();
            } else if (key_pressed == '2') {
                Student_Management();
            } else if (key_pressed == '3') {
                 View_Present_Students();
                 return_to_main_menu();   
            } else if (key_pressed == '4') {
                display_temperature_continuously(7); 
                return_to_main_menu();
            } else if (key_pressed == '5') {
                Retrieve_Student_Data(); 
                return_to_main_menu();

            } else if (key_pressed == '6') {
                key_pressed = 0;
                LCD_clear();
                LCD_string("Press * to exit");
                _delay_ms(500);
                Traffic_Monitoring();
                return_to_main_menu();
            } else if (key_pressed == '#') {
                current_page++;
                if (current_page > 3) current_page = 1;
                menu_needs_update = 1;
            } else if (key_pressed == '*') {
                current_page--;
                if (current_page < 1) current_page = 3;
                menu_needs_update = 1;
            } else {
                invalid_key_pressed();
                return_to_main_menu();
            }
            key_pressed = 0;
        }
    }

    return 0;
}
//**********************************************************************************************************
void display_menu(int page) {
    LCD_clear();
    if (page == 1) {
        LCD_string("1. Attendance Initialization");
        LCD_command(0xC0);
        LCD_string("2. Student Management");
    } else if (page == 2) {
        LCD_string("3. View Present Students");
        LCD_command(0xC0);
        LCD_string("4. Temperature Monitoring");
    } else if (page == 3) {
        LCD_string("5. Retrieve Student Data");
        LCD_command(0xC0);
        LCD_string("6. Traffic Monitoring");
    } 
}
//**********************************************************************************************************
void display_Attendance_Ready_menu_page(){
    LCD_clear();
    LCD_string("1.Submit Student Code");
    LCD_command(0xC0);
    LCD_string("2.EXIT");

}
//**********************************************************************************************************
void Submit_Student_Code() {
    unsigned long int studentCode = get_student_id();

    if ( handle_student_search(studentCode)) {
        LCD_clear();
        LCD_string("Attendance already recorded.");
        _delay_ms(1000);
        return;
    }

    LCD_clear();
    LCD_string("Code submitted");
    WRITE_STU_ID_IN_EEPROM(studentCode);
    _delay_ms(1000);
}

//**********************************************************************************************************
bool check_stu_id(char *stuId) {
    if (strlen(stuId) != 8 || strncmp(stuId, "401", 3) != 0)
        return false;
    return true;
}
//**********************************************************************************************************
void Wrong_format_of_stu_ID() {
    LCD_clear();
    LCD_string("wrong format !");
    BUZZER();
    _delay_ms(1000);
}
//**********************************************************************************************************
void BUZZER() {
    DDRB |= (1 << 2);
    PORTB |= (1 << 2);
    _delay_ms(1000);
    PORTB &= ~(1 << 2);
}
//**********************************************************************************************************
unsigned long int get_student_id(void) {
    char Student_Code[9] = {0}; 
    unsigned char i = 0;
    LCD_clear();
    LCD_string("Enter Student Code:");
    while (1) {
        key_pressed = 0;

        while (key_pressed == 0);

        // Handle '#' as the end of input
        if (key_pressed == '#') {
            if (check_stu_id(Student_Code)) { 
                return atol(Student_Code);   
            } else {
                Wrong_format_of_stu_ID(); 
                memset(Student_Code, 0, sizeof(Student_Code)); 
                i = 0; 
                LCD_clear();
                LCD_string("Enter Student Code:"); 
            }
        } else if (key_pressed >= '0' && key_pressed <= '9') {
            if (i < 8) { 
                Student_Code[i++] = key_pressed;
                LCD_data(key_pressed); 
            } else { // If user enters more than 8 digits
                Wrong_format_of_stu_ID();
                memset(Student_Code, 0, sizeof(Student_Code)); // Reset input buffer
                i = 0; // Reset index
                LCD_clear();
                LCD_string("Enter Student Code:"); 
            }
        } else { // Handle invalid characters
            Wrong_format_of_stu_ID();
            memset(Student_Code, 0, sizeof(Student_Code)); 
            i = 0; 
            LCD_clear();
            LCD_string("Enter Student Code:"); 
        }
    }
}
//**********************************************************************************************************
void Student_Management(void) {
    int current_page = 1;
    while (1) {
        display_student_management_menu_page(current_page);

        key_pressed = 0;

        while (key_pressed == 0);

        if (key_pressed == '1' && current_page == 1) {
            unsigned long int studentCode = get_student_id();
            if (handle_student_search(studentCode)) {
                LCD_command(0xC0);
                LCD_string("Student Present!");
                _delay_ms(1000);
            } else {
                LCD_command(0xC0);
                LCD_string("Student Absent!");
                _delay_ms(1000);
            }
        } else if (key_pressed == '2' && current_page == 1) {
            unsigned long int studentCode = get_student_id();
            if (delete_student_from_eeprom(studentCode)) {
                LCD_command(0xC0);
                LCD_string("Student Deleted!");
                _delay_ms(1000);
            } else {
                LCD_command(0xC0);
                LCD_string("Not Found!");
                _delay_ms(1000);
            }
        } else if (key_pressed == '3') {
            return_to_main_menu();
            break;
        } else if (key_pressed == '#') {
            current_page++;
            if (current_page > 2) {
                current_page = 1;
            }
        } else if (key_pressed == '*') {
            current_page--;
            if (current_page < 1) {
                current_page = 2;
            }
        } else {
            invalid_key_pressed();
        }
    }
}
//**********************************************************************************************************
void display_student_management_menu_page(int page) {
    LCD_clear();
    switch (page) {
        case 1:
            LCD_string("1.Search Students");
            LCD_command(0xC0);
            LCD_string("2.Delete Student");
            break;
        case 2:
            LCD_string("3.Exit");
    }
}
//**********************************************************************************************************
bool handle_student_search(unsigned long int stuID) {
    uint16_t num_of_students = GET_NUM_OF_STUDENTS(); 

    for (uint16_t i = 0; i < num_of_students; i++) {
        unsigned long int student_id = READ_STU_ID_FROM_EEPROM(i); 
        if (student_id == stuID) {
            return true; 
        }
    }
    return false;
}
//**********************************************************************************************************
void return_to_main_menu(void) {
    LCD_clear();
    display_menu(current_page);
    key_pressed = 0;
}
//**********************************************************************************************************
void invalid_key_pressed(void) {
    LCD_clear();
    LCD_string("Invalid Key!");
    BUZZER();
    _delay_ms(1000);
  
}
//**********************************************************************************************************
void Attendance_Ready() {
    bool time_set = false;
    while (1) {
        if(!time_set){
            LCD_clear();
            uint32_t attendance_time = get_attendance_time();
            init_timer(attendance_time);
            time_set = true;
        }
        
        display_Attendance_Ready_menu_page();
        
        // Reset key press state
        key_pressed = 0;
        // Wait for a key press or timer expiry
        while (key_pressed == 0 && !is_time_up());

        // If time is up, exit the loop
        if (is_time_up()) {
            LCD_clear();
            LCD_string("Time's up!");
            _delay_ms(2000);
            return_to_main_menu();
            return;
        }

        // Handle key press actions
        switch (key_pressed) {
            case '1':
                LCD_clear();
                Submit_Student_Code();
                break;

            case '2':
                TCCR0 = 0; 
                return_to_main_menu();
                return; // Exit the function

            default:
                invalid_key_pressed();
                break;
        }
    }
}
//**********************************************************************************************************
void View_Present_Students(){
    
    LCD_clear();
    display_students();

}
//**********************************************************************************************************
void display_students() {
    uint16_t number_of_students = GET_NUM_OF_STUDENTS();
    char arr[50];

    // Display the number of present students
    sprintf(arr, "%d", number_of_students);
    LCD_string("Present students: ");
    LCD_string(arr);
    _delay_ms(1000);
    LCD_clear();

    // If there are students, display their IDs
    if (number_of_students != 0) {
        for (uint16_t i = 0; i < number_of_students; i++) {
            unsigned long int student_id = READ_STU_ID_FROM_EEPROM(i); 
            sprintf(arr, "id : %ld", student_id);
            LCD_string(arr);
            _delay_ms(1000);
            LCD_clear();
        }
        LCD_string("All students displayed");
        _delay_ms(500);
    } else {
        LCD_string("No students submitted yet");
        _delay_ms(500);
    }
}

//**********************************************************************************************************
void Retrieve_Student_Data() {

    int number_of_students = GET_NUM_OF_STUDENTS();

    // If no students have submitted data
    if (number_of_students == 0) {
        LCD_string("No students submitted yet");
        _delay_ms(1000);
        return;
    } else {
        LCD_string("Retrieving student data...");
        _delay_ms(1000);  // Delay to show the message

        char arr[9];

        // Loop through each student and retrieve their ID
        for (uint16_t i = 0; i < number_of_students; i++) {
            unsigned long int student_id = READ_STU_ID_FROM_EEPROM(i); 
            sprintf(arr, "%ld", student_id);
            USART_Transmit_string(arr);
            USART_Transmit_Data('\r');
            _delay_ms(500);
        }

        // Indicate the end of the student data
        USART_Transmit_string("-----------------\r");
        _delay_ms(500);

        // Display a success message on the LCD
        LCD_clear();
        LCD_string("Data Sent Successfully!");
        _delay_ms(1000);
    }
}

//**********************************************************************************************************
uint32_t get_attendance_time() {
    
    char Time_Input[5] = {0}; // Format: MMSS
    unsigned char i = 0;

    LCD_clear();
    LCD_string("Enter Time (MMSS):");

    while (1) {
        key_pressed = 0;

        // Wait for key press
        while (key_pressed == 0);

        // Handle '#' as the end of input
        if (key_pressed == '#') {
            if (i == 4) { // Ensure the input is 4 digits long
                LCD_clear();
                LCD_string("Time recorded!");
                _delay_ms(500);
                
                // Convert MMSS format to total seconds
                uint32_t minutes = (Time_Input[0] - '0') * 10 + (Time_Input[1] - '0');
                uint32_t seconds = (Time_Input[2] - '0') * 10 + (Time_Input[3] - '0');

                // Set the timer with total seconds (convert to milliseconds)
                init_timer((minutes * 60 * 1000) + (seconds * 1000)); 
                
                // Return total time in seconds
                return (minutes * 60 + seconds);
            } else { // Invalid input length
                LCD_clear();
                LCD_string("Invalid Time!");
                _delay_ms(2000);
                memset(Time_Input, 0, sizeof(Time_Input)); // Reset input buffer
                i = 0; // Reset index
                LCD_clear();
                LCD_string("Enter Time (MMSS): ");
            }
        } else if (key_pressed >= '0' && key_pressed <= '9') {
            if (i < 4) { // Accept up to 4 digits
                Time_Input[i++] = key_pressed;
                LCD_data(key_pressed); // Display the input digit on LCD
            } else { // If more than 4 digits are entered
                LCD_clear();
                LCD_string("Invalid Time!");
                _delay_ms(2000);
                memset(Time_Input, 0, sizeof(Time_Input)); // Reset input buffer
                i = 0; // Reset index
                LCD_clear();
                LCD_string("Enter Time (MMSS): ");
            }
        } else { // Handle invalid characters
            LCD_clear();
            LCD_string("ERROR!");
            _delay_ms(2000);
            memset(Time_Input, 0, sizeof(Time_Input)); // Reset input buffer
            i = 0; // Reset index
            LCD_clear();
            LCD_string("Enter Time (MMSS): ");
        }
    }
}

//**********************************************************************************************************
void Traffic_Monitoring() {
    char numberString[16];  // Buffer to hold distance string
    uint16_t pulseWidth;    // Pulse width from echo
    int distance;           // Calculated distance
    int previous_count = -1;  // Store previous count to avoid unnecessary updates

    HCSR04Init();           // Initialize ultrasonic sensor

    while (1) {
        if (key_pressed == '*') {
            break;  // Exit the loop if '#' is pressed
        }
        _delay_ms(100);  // Delay for sensor stability

        HCSR04Trigger();              // Send trigger pulse
        pulseWidth = GetPulseWidth(); // Measure echo pulse

        if (pulseWidth == US_ERROR) {
            LCD_clear();
            LCD_string("Error");  // Display error message
        } else if (pulseWidth == US_NO_OBSTACLE) {
            LCD_clear();
            LCD_string("No Obstacle");  // Display message when no obstacle is detected
        } else {
            // Calculate the distance in cm
            distance = (int)((pulseWidth * 0.034 / 2) + 0.5);

            // Display distance on LCD
            sprintf(numberString, "%d", distance); // Convert distance to string
            LCD_clear();
            LCD_string("Distance: ");
            LCD_string(numberString); 
            LCD_string(" cm");

            // Counting logic based on distance
            if (distance < 6) {
                count++;  // Increment count if distance is below threshold
            }

            // Update count on LCD only if it changes
            if (count != previous_count) {
                previous_count = count;
                sprintf(numberString, "%d", count);
                LCD_command(0xC0);  // Move cursor to the second line of LCD
                LCD_string("Count: ");
                LCD_string(numberString);
                _delay_ms(500);
            } else {
                _delay_ms(500);  
            }
        }
    }
}
//**********************************************************************************************************