#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctype.h>

#include "../headers/input.h"

struct termios original_term;

void restore_terminal(){
	tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
}


void enable_non_blocking_input() {
	tcgetattr(STDIN_FILENO, &original_term);
	atexit(restore_terminal);

	struct termios ttystate = original_term;

	ttystate.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

	//fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void disable_non_blocking_input() {
	struct termios ttystate;

	tcgetattr(STDIN_FILENO, &ttystate);
	ttystate.c_lflag |= (ICANON | ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}
