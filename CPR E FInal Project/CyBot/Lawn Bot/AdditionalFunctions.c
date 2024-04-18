#include "AdditionalFunctions.h"
#include "ping_template.h"

void printString2(char something[]) {
    int j;
    for(j = 0; j < strlen(something); j++) {
        lcd_putc(something[j]);
    }
}

void makeString(char* string, int number) {
    *(string + 7) = '\0';
    *(string + 6) = (char)((number % 10) + 48);
    *(string + 5) = (char)(((number / 10) % 10) + 48);
    *(string + 4) = (char)(((number / 100) % 10) + 48);
    *(string + 3) = (char)(((number / 1000) % 10) + 48);
    *(string + 2) = (char)(((number / 10000) % 10) + 48);
    *(string + 1) = (char)(((number / 100000) % 10) + 48);
    *(string + 0) = (char)(((number / 1000000) % 10) + 48);
}

int calculateDistance(int value) {
    int results = round(1.0 / ((value / 4095.0) * .13));
    if(results > 80) {
        return 80;
    } else {
        return results;
    }
}

int calculatePingDis() {
    double divide = RESULTS / 1.6E7;
    double mult = 343 * divide;
    double half = mult / 2;
    double cm = half * 100;
    return (int)cm;
}
