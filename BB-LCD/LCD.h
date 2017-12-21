/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   LCD.h
 * Author: Salvador
 *
 * Created on October 6, 2017, 11:49 AM
 */

#ifndef LCD_H
#define LCD_H

#include <string>
using std::string;

class LCD {
private:
    FILE* clock_gpio = fopen("/sys/class/gpio/gpio2/value", "w");
    FILE* latch_gpio = fopen("/sys/class/gpio/gpio5/value", "w");
    FILE* serial_gpio = fopen("/sys/class/gpio/gpio4/value", "w");
    FILE* enable_gpio = fopen("/sys/class/gpio/gpio27/value", "w");
    FILE* write_gpio = fopen("/sys/class/gpio/gpio65/value", "w");
    int function_array[8] = {0, 0, 1, 1, 0, 0, 0, 0};
    int display_array[8] = {0, 0, 0, 0, 1, 1, 1, 1};
    int clear_array[8] = {0, 0, 0, 0, 0, 0, 0, 1};

    void delay_us(int);
    void clearValues(void);
    void clockValue(void);
    void setSerialValue(int);
    void setLatchValue(int);
    void toggleEnable();
    void initializeScreen();
    void writeValues(int[]);
    void writeString(string);
    string getLookupValue(string);


public:
    LCD();
    ~LCD();
    void clear();
    void write(string);
};

#endif /* LCD_H */

