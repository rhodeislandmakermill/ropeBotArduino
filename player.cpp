#include "player.h"
#include "Arduino.h"

Player::Player(int top, int bottom, int out, int up, int down ) {
	//Set pins variables
	topStopPin = top;
	bottomStopPin = bottom;
	outPin = out;
	upButtonPin = up;
	downButtonPin = down;

	//Initialize pin mode
	initializePins();

	//Default player to offmark state
	state = offmark;
}


// PRIVATE FUNCTIONS

void Player::initializePins() {
	pinMode( topStopPin, INPUT);
	pinMode( bottomStopPin, INPUT);
	pinMode( outPin, OUTPUT);
	pinMode( upButtonPin, INPUT );
	pinMode( downButtonPin, INPUT );
}

