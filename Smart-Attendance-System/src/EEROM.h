#ifndef EEPROM_H
#define EEPROM_H

#include <avr/io.h>
#include <stdbool.h>

// تعریف آدرس‌های مربوط به EEPROM
#define STUDENT_START_ADDRESS 0x0000   // شروع ذخیره شماره‌های دانشجویی
#define NUM_OF_STUDENTS_HIGH_BYTE 0x03FE // بایت بالای تعداد دانشجویان
#define NUM_OF_STUDENTS_LOW_BYTE 0x03FF  // بایت پایین تعداد دانشجویان

// توابع مدیریت EEPROM
void EEPROM_Write(uint16_t address, uint8_t data);     // نوشتن یک بایت در EEPROM
uint8_t EEPROM_Read(uint16_t address);                // خواندن یک بایت از EEPROM

// توابع مربوط به شماره‌های دانشجویی
void WRITE_STU_ID_IN_EEPROM(unsigned long int stuID);  // نوشتن شماره دانشجویی در EEPROM
unsigned long int READ_STU_ID_FROM_EEPROM(uint16_t index); // خواندن شماره دانشجویی از EEPROM با ایندکس
uint16_t GET_NUM_OF_STUDENTS();                       // گرفتن تعداد دانشجویان ذخیره‌شده
void SET_NUM_OF_STUDENTS(uint16_t num_of_students);   // تنظیم تعداد دانشجویان ذخیره‌شده
bool handle_student_search(unsigned long int stuID);  // جستجوی شماره دانشجویی

#endif
