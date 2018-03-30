#include "player.h"

Player::Player(int top, int bottom, int out, int up, int down ) {
	topStopPin = top;
	bottomStopPin = bottom;
	outPin = out;
	upButtonPin = up;
	downButtonPin = down;

	state = offmark;
}

