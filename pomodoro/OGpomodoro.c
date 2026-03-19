#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sensehat.h"

//Defines
#define Work (25*60)
#define Break (5*60)

void timer(int time, const char * typeOfSession){
    int seconds = 0;

    for(int timer =0;timer < time; timer++){

        int minutes = (seconds % 3600) / 60;
        int internalSeconds= seconds%60;
        printf("%s %02d:%02d\n", typeOfSession, minutes, internalSeconds);
        sleep(1);
        seconds++;


    }



}
int main(void){

    while(1){
        timer(Work,"Work:");
        printf("Work is done!\n");

        timer(Break,"Break:");
        printf("Break is done!\n");

    }

    return 0;
}
