#include "player.h"
#include "Arduino.h"

Player::Player() {
	//Set pins variables
	topStopPin = -1;
	bottomStopPin = -1;
	outPin = -1;
	upButtonPin = -1;
	downButtonPin = -1;

	//Default player to offmark state
	state = offmark;
}

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

void Player::updateState( bool raceBegun ) {
		switch( state ) {
		case offmark:
			//Turn off motors
			if( digitalRead(bottomStopPin) == HIGH ) {	
				state = onmark;
			}
			break;
		case onmark:
			if( digitalRead(bottomStopPin) == LOW ) {
				if( raceBegun == false ) {
					state = offmark;	
				} else {
					state = started;
				}
			}			
			break;			
		case started:
			//Turn on motors
			if( digitalRead(topStopPin) == HIGH ) {
				state = halfway;
			}
			break;
		case halfway:
			if( digitalRead(bottomStopPin) == HIGH ) {
				state = finished;
			}
			break;
		case finished:
			//Turn off motors
			break;
	}
}

const char* Player::getState() {
	return stateNames[state];
}

bool Player::isOnMark() {
	return state == onmark;
}

bool Player::isFinished() {
	return state == finished;
}

void Player::setOut(bool value) {
	digitalWrite(outPin, value);
}

void Player::reset() {
	state = offmark;
	digitalWrite(outPin, LOW);
}


// PRIVATE FUNCTIONS

void Player::initializePins() {
	pinMode( topStopPin, INPUT);
	pinMode( bottomStopPin, INPUT);
	pinMode( outPin, OUTPUT);
	pinMode( upButtonPin, INPUT );
	pinMode( downButtonPin, INPUT );
}

