// pomodoro.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sensehat.h"

// Defines
#define Work (1*60)  // 25 minutes
#define Break (1*60)  // 5 minutes

// Timer function
void timer(int time, const char *typeOfSession) {
    int seconds = 0;
    uint16_t color;
    sense_clear();

    if (strcmp(typeOfSession, "Work:") == 0) {
        color = RED;
    } else {
        color = GREEN;
    }

    for (int t = 0; t < time; t++) {
        int minutes = (seconds % 3600) / 60;
        int internalSeconds = seconds % 60;
        printf("%s %02d:%02d\n", typeOfSession, minutes, internalSeconds);

        // Calculate LED progress
        int led_index = (seconds * LED_COUNT) / time;
        sense_light_led(led_index, color);

        sleep(1);
        seconds++;
    }
}

// Simple beep using system bell (\a)
void beep_work() {
    // Work done: 3 short bells
    printf("\a\a\a");
    fflush(stdout);
}

void beep_break() {
    // Break done: 2 long bells
    printf("\a");
    fflush(stdout);
    usleep(400000);  // 0.4s pause
    printf("\a");
    fflush(stdout);
}

int main(void) {
    // Initialize Sense HAT
    if (sense_init("/dev/fb0") < 0) {
        return 1;
    }
    sense_clear();

    while (1) {
        timer(Work, "Work:");
        printf("Work is done!\n");
        beep_work();

        timer(Break, "Break:");
        printf("Break is done!\n");
        beep_break();
    }

    sense_close();
    return 0;
}
