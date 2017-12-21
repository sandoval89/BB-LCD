#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include <fstream>
#include <iostream>
using std::string;
using std::cin;

#include "LCD.h"

int main() {
    string input_str;
    LCD *screen = new LCD();

    do {
        printf("Enter string: ");
        getline(cin, input_str);
        if (input_str.compare("exit") == 0) {
            break;
        }
        screen->clear();
        screen->write(input_str);
    } while (true);

    delete screen;

    return 0;
}