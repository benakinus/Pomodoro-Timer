#makefile for pomodoro.c
a.out: pomodoro.c sensehat.o
	gcc -g pomodoro.c sensehat.o -li2c -lm
sensehat.o: sensehat.c sensehat.h
	gcc -g sensehat.c -g -c -o sensehat.o	
.PHONY: clean
clean:
	rm -f *.o
	touch *

