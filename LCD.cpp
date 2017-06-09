#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class LCD {
	private:
		FILE* clock_gpio = fopen("/sys/class/gpio/gpio2/value","w");
		FILE* latch_gpio = fopen("/sys/class/gpio/gpio5/value","w");
		FILE* serial_gpio = fopen("/sys/class/gpio/gpio4/value","w");
		FILE* enable_gpio = fopen("/sys/class/gpio/gpio27/value","w");
		FILE* write_gpio = fopen("/sys/class/gpio/gpio65/value","w");
		int function_array[8] = {0,0,1,1,0,0,0,0};
		int display_array[8] = {0,0,0,0,1,1,1,1};
		int clear_array[8] = {0,0,0,0,0,0,0,1};

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

int main() {

	return 0;
}

LCD::LCD() {
	initializeScreen();
}

LCD::~LCD() {
	fclose(clock_gpio);
	fclose(latch_gpio);
	fclose(serial_gpio);
	fclose(enable_gpio);
	fclose(write_gpio);
}

void LCD::initializeScreen() {
	FILE* enable_direction_gpio = fopen("/sys/class/gpio/gpio27/direction","w");
	FILE* write_direction_gpio = fopen("/sys/class/gpio/gpio65/direction","w");

	fseek(enable_direction_gpio, 0, SEEK_SET);
	fprintf(enable_direction_gpio, "out\n");
	fseek(enable_direction_gpio, 0, SEEK_SET);
	delay_us(500);

	fseek(write_direction_gpio, 0, SEEK_SET);
	fprintf(write_direction_gpio, "out\n");
	fseek(write_direction_gpio, 0, SEEK_SET);
	delay_us(500);

	fclose(enable_direction_gpio);
	fclose(write_direction_gpio);

	fseek(write_gpio, 0, SEEK_SET);
	fprintf(write_gpio, "%d\n", 0);
	fseek(write_gpio, 0, SEEK_SET);
	delay_us(500);

	fseek(enable_gpio, 0, SEEK_SET);
	fprintf(enable_gpio, "%d\n", 0);
	fseek(enable_gpio, 0, SEEK_SET);
	delay_us(500);

	//clear display
	clear();

	//set function
	writeValues(function_array);

	//set display
	writeValues(display_array);
}

void LCD::clear() {
	writeValues(clear_array);
}

void LCD::writeValues(int dpValues[]) {
	clearValues();
	setLatchValue(0);

	for(int i = 0; i < 8; i++) {
		if( i == 0 || dpValues[i] != dpValues[i-1])
		       setSerialValue(dpValues[i]);
		clockValue();
	}

	setLatchValue(1);
	clockValue();

	toggleEnable();
}

void LCD::write(string message) {
	fseek(write_gpio, 0, SEEK_SET);
	fprintf(write_gpio, "%d\n", 1);
	fseek(write_gpio, 0, SEEK_SET);
	delay_us(500);

	writeString(message);

	fseek(write_gpio, 0, SEEK_SET);
	fprintf(write_gpio, "%d\n", 0);
	fseek(write_gpio, 0, SEEK_SET);
	delay_us(500);
}

void LCD::writeString(string character_string) {
	string value_string, bit_string;
	int value_array[8];
	for(int i = 0; i < character_string.length(); i++) {
        value_string = getLookupValue(character_string.substr(i,1));
		for(int j = 0; j < value_string.length(); i++) {
			bit_string = value_string.substr(i,1);
			if(bit_string.compare("0") == 0)
				value_array[j] = 0;
			else
				value_array[j] = 1;
		}
		writeValues(value_array);
    }
}

string LCD::getLookupValue(string target_character) {
    if(target_character.compare(" ") == 0)
        return "00100000";

    ifstream myFile("characterTable.txt");
    string character, number;
    while(getline(myFile, character)) {
        getline(myFile, number);
        if(target_character.compare(character) == 0) {
            break;
        }
    }
    myFile.close();
    return number;
}

void LCD::toggleEnable() {
	fseek(enable_gpio, 0, SEEK_SET);
	fprintf(enable_gpio, "%d\n", 1);
	fseek(enable_gpio, 0, SEEK_SET);
	delay_us(500);

	fseek(enable_gpio, 0, SEEK_SET);
	fprintf(enable_gpio, "%d\n", 0);
	fseek(enable_gpio, 0, SEEK_SET);
	delay_us(500);
};

void LCD::clearValues() {
	fseek(clock_gpio, 0, SEEK_SET);
	fprintf(clock_gpio, "%d\n", 1);
	fseek(clock_gpio, 0, SEEK_SET);
	delay_us(500);

	fseek(latch_gpio, 0, SEEK_SET);
	fprintf(latch_gpio, "%d\n", 1);
	fseek(latch_gpio, 0, SEEK_SET);
	delay_us(500);

	fseek(serial_gpio, 0, SEEK_SET);
	fprintf(serial_gpio, "%d\n", 1);
	fseek(serial_gpio, 0, SEEK_SET);
	delay_us(500);
}

void LCD::clockValue() {
	fseek(clock_gpio, 0, SEEK_SET);
	fprintf(clock_gpio, "%d\n", 0);
	fseek(clock_gpio, 0, SEEK_SET);
	delay_us(500);

	fseek(clock_gpio, 0, SEEK_SET);
	fprintf(clock_gpio, "%d\n", 1);
	fseek(clock_gpio, 0, SEEK_SET);
	delay_us(500);
}

void LCD::setSerialValue(int value) {
	fseek(serial_gpio, 0, SEEK_SET);
	fprintf(serial_gpio, "%d\n", value);
	fseek(serial_gpio, 0, SEEK_SET);
	delay_us(500);
}

void LCD::setLatchValue(int value) {
	fseek(latch_gpio, 0, SEEK_SET);
	fprintf(latch_gpio, "%d\n", value);
	fseek(latch_gpio, 0, SEEK_SET);
	delay_us(500);
}

void LCD::delay_us(int desired_delay_us){
// Function obtained from pp. 228-229 of:
// Barrett and Kridner, "Bad to the Bone", Morgan&Claypool Publishers, 2013

	struct timeval tv_start;	// start time hack
	struct timeval tv_now;		//current time hack
	int elapsed_time_us;

	gettimeofday(&tv_start,NULL);
	elapsed_time_us = 0;

	while(elapsed_time_us < desired_delay_us) {
		gettimeofday(&tv_now,NULL);
		if(tv_now.tv_usec >= tv_start.tv_usec)
			elapsed_time_us = tv_now.tv_usec - tv_start.tv_usec;
		else
			elapsed_time_us = (1000000 - tv_start.tv_usec) + tv_now.tv_usec;
	}
	return;
}
