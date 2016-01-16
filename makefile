#Copyright:	Fabian Ebner (2011)
#Author:	Fabian Ebner
#Licence:	GPL 3.0
#Created:	16.08.2011
#Last change:	1.10.2011
#This is free software. You can redistribute it under certain conditions (see LICENSE)
all: main.o terminal.o input.o output.o shot.o ship.o interaction.o menu.o
	gcc -Wall -g -ansi -std=c99 -o konsole_invaders main.o terminal.o input.o output.o shot.o ship.o interaction.o menu.o -pthread;
main.o: main.c terminal.c terminal.h input.c input.h output.c output.h types.h ship.c ship.h shot.c shot.h interaction.c interaction.h menu.c menu.h config.h
	gcc -Wall -g -ansi -std=c99 -c -o main.o main.c;
terminal.o: terminal.c terminal.h output.c output.h config.h
	gcc -Wall -g -ansi -std=c99 -c -o terminal.o terminal.c;
input.o: input.c input.h terminal.c terminal.h output.c output.h config.h
	gcc -Wall -g -ansi -std=c99 -c -o input.o input.c -pthread;
output.o: output.c output.h config.h
	gcc -Wall -g -ansi -std=c99 -c -o output.o output.c;
shot.o: shot.c shot.h interaction.c interaction.h types.h output.c output.h config.h
	gcc -Wall -g -ansi -std=c99 -c -o shot.o shot.c;
ship.o: ship.c ship.h shot.c shot.h interaction.c interaction.h output.c output.h types.h config.h
	gcc -Wall -g -ansi -std=c99 -c -o ship.o ship.c;
interaction.o: interaction.c interaction.h ship.c ship.h shot.c shot.h output.c output.h types.h config.h
	gcc -Wall -g -ansi -std=c99 -c -o interaction.o interaction.c;
menu.o: menu.c menu.h output.c output.h config.h input.c input.h
	gcc -Wall -g -ansi -std=c99 -c -o menu.o menu.c;
clean:
	rm -f konsole_invaders *.o *~ .konsole_invaders.conf
