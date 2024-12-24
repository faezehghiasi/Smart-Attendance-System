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

void display_menu(int page);
void Attendance_Ready();
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


int main(void) {
    LCD_init();
    keypad_init();
    init_interrupt();
    SET_NUM_OF_STUDENTS(0); // Initialize the number of students in EEPROM
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
                while (1) {
                    LCD_clear();
                    Attendance_Ready();
                    key_pressed = 0;
                    while (key_pressed == 0);
                    if (key_pressed == '1') {
                        LCD_clear();
                        Submit_Student_Code();
                    } else if (key_pressed == '2') {
                        return_to_main_menu();
                        break;
                    } else {
                        invalid_key_pressed();
                    }
                }
            } else if (key_pressed == '2') {
                Student_Management();
            } else if (key_pressed == '3') {
                display_all_students();
                return_to_main_menu();
            } else if (key_pressed == '4') {
                display_temperature();
                return_to_main_menu();
            } else if (key_pressed == '5') {
                LCD_string("Retrieve Data");
            } else if (key_pressed == '6') {
                LCD_string("Traffic Monitor");
            } else if (key_pressed == '#') {
                current_page++;
                if (current_page > 3) current_page = 1;
                menu_needs_update = 1;
            } else if (key_pressed == '*') {
                current_page--;
                if (current_page < 1) current_page = 3;
                menu_needs_update = 1;
            } else {
                LCD_string("Invalid Key");
            }
            key_pressed = 0;
        }
    }

    return 0;
}

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
    } else {
        LCD_clear();
        LCD_string("Invalid Page");
        _delay_ms(2000);
    }
}

void Attendance_Ready() {
    LCD_clear();
    LCD_string("1.Submit Student Code");
    LCD_command(0xC0);
    LCD_string("2.EXIT");
}

void Submit_Student_Code() {
    unsigned long int studentCode = get_student_id();
    LCD_command(0xC0);
    LCD_string("Code submitted");
    WRITE_STU_ID_IN_EEPROM(studentCode);
    _delay_ms(20);
}

bool check_stu_id(char *stuId) {
    if (strlen(stuId) != 8 || strncmp(stuId, "401", 3) != 0)
        return false;
    return true;
}

void Wrong_format_of_stu_ID() {
    BUZZER();
    LCD_command(0xC0);
    LCD_string("Your Student Code format is wrong");
    _delay_ms(20);
}

void BUZZER() {
    DDRB |= (1 << 2);
    PORTB |= (1 << 2);
    _delay_ms(50);
    PORTB &= ~(1 << 2);
}

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
                _delay_ms(200);
            } else {
                LCD_command(0xC0);
                LCD_string("Student Absent!");
                _delay_ms(200);
            }
        } else if (key_pressed == '2' && current_page == 1) {
            unsigned long int studentCode = get_student_id();
            if (delete_student_from_eeprom(studentCode)) {
                LCD_command(0xC0);
                LCD_string("Student Deleted!");
                _delay_ms(200);
            } else {
                LCD_command(0xC0);
                LCD_string("Not Found!");
                _delay_ms(200);
            }
        } else if (key_pressed == '3' && current_page == 2) {
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

void return_to_main_menu(void) {
    LCD_clear();
    display_menu(current_page);
    key_pressed = 0;
}

void invalid_key_pressed(void) {
    LCD_clear();
    LCD_string("Invalid Key!");
    _delay_ms(100);
    display_menu(current_page);
}


