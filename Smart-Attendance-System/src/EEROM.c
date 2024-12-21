
#include "EEROM.h"

// نوشتن یک بایت در EEPROM
void EEPROM_Write(uint16_t address, uint8_t data) {
    while (EECR & (1 << EEWE));  // انتظار برای اتمام عملیات نوشتن قبلی
    EEARH = (address >> 8);      // تنظیم بایت بالای آدرس
    EEARL = (address & 0xFF);    // تنظیم بایت پایین آدرس
    EEDR = data;                 // تنظیم داده
    EECR |= (1 << EEMWE);        // مجوز نوشتن
    EECR |= (1 << EEWE);         // شروع عملیات نوشتن
}

// خواندن یک بایت از EEPROM
uint8_t EEPROM_Read(uint16_t address) {
    while (EECR & (1 << EEWE));  // انتظار برای اتمام عملیات نوشتن قبلی
    EEARH = (address >> 8);      // تنظیم بایت بالای آدرس
    EEARL = (address & 0xFF);    // تنظیم بایت پایین آدرس
    EECR |= (1 << EERE);         // شروع عملیات خواندن
    return EEDR;                 // بازگرداندن داده
}


// نوشتن شماره دانشجویی در EEPROM
void WRITE_STU_ID_IN_EEPROM(unsigned long int stuID) {
    uint16_t num_of_students = GET_NUM_OF_STUDENTS();  // تعداد دانشجویان ذخیره‌شده
    uint16_t address = STUDENT_START_ADDRESS + (num_of_students * 4);  // محاسبه آدرس جدید

    // نوشتن شماره دانشجویی در 4 بایت
    EEPROM_Write(address, (stuID >> 24) & 0xFF);      // بایت اول
    EEPROM_Write(address + 1, (stuID >> 16) & 0xFF);  // بایت دوم
    EEPROM_Write(address + 2, (stuID >> 8) & 0xFF);   // بایت سوم
    EEPROM_Write(address + 3, stuID & 0xFF);          // بایت چهارم

    // به‌روزرسانی تعداد دانشجویان
    num_of_students++;
    SET_NUM_OF_STUDENTS(num_of_students);
}

// خواندن شماره دانشجویی از EEPROM
unsigned long int READ_STU_ID_FROM_EEPROM(uint16_t index) {
    uint16_t address = STUDENT_START_ADDRESS + (index * 4);  // محاسبه آدرس بر اساس ایندکس
    unsigned long int stuID = 0;

    // خواندن 4 بایت شماره دانشجویی
    stuID |= ((unsigned long int)EEPROM_Read(address) << 24);     // بایت اول
    stuID |= ((unsigned long int)EEPROM_Read(address + 1) << 16); // بایت دوم
    stuID |= ((unsigned long int)EEPROM_Read(address + 2) << 8);  // بایت سوم
    stuID |= (unsigned long int)EEPROM_Read(address + 3);         // بایت چهارم

    return stuID;
}

// گرفتن تعداد دانشجویان ذخیره‌شده
uint16_t GET_NUM_OF_STUDENTS() {
    uint16_t num_of_students = 0;
    uint8_t high_byte = EEPROM_Read(NUM_OF_STUDENTS_HIGH_BYTE);  // خواندن بایت بالا
    uint8_t low_byte = EEPROM_Read(NUM_OF_STUDENTS_LOW_BYTE);    // خواندن بایت پایین

    // ترکیب دو بایت برای بدست آوردن تعداد دانشجویان
    num_of_students = (high_byte << 8) | low_byte;
    return num_of_students;
}

// تنظیم تعداد دانشجویان ذخیره‌شده
void SET_NUM_OF_STUDENTS(uint16_t num_of_students) {
    EEPROM_Write(NUM_OF_STUDENTS_HIGH_BYTE, (num_of_students >> 8) & 0xFF);  // بایت بالا
    EEPROM_Write(NUM_OF_STUDENTS_LOW_BYTE, num_of_students & 0xFF);          // بایت پایین
}

void GET_ARRAY_OF_STUDENTS(unsigned long int *students) {
    uint16_t num_of_students = GET_NUM_OF_STUDENTS();  // تعداد دانشجویان ذخیره‌شده
    for (uint16_t i = 0; i < num_of_students; i++) {
        students[i] = READ_STU_ID_FROM_EEPROM(i);  // خواندن هر شماره دانشجویی از EEPROM
    }
}


