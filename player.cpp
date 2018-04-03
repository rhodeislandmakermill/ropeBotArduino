#include "player.h"
#include "Arduino.h"

#define DEFAULT_SPEED 128

Player::Player() {
	//Set pins variables
	topStopPin = -1;
	bottomStopPin = -1;
	outPin = -1;
	upButtonPin = -1;
	downButtonPin = -1;
	currentSpeed = 0;

	//Default player to offmark state
	state = offmark;
}

Player::Player(int top, int bottom, int out, int up, int down, Motor* theMotor ) {
	//Set pins variables
	topStopPin = top;
	bottomStopPin = bottom;
	outPin = out;
	upButtonPin = up;
	downButtonPin = down;
	currentSpeed = 0;
	motor = theMotor;

	//Initialize pin mode
	initializePins();

	//Default player to offmark state
	state = offmark;
	previousState = finished;
	controlsEnabled = false;
}

void Player::updateState() {

	if( controlsEnabled ) {
		if( digitalRead( upButtonPin ) == HIGH ) {
			upPressed();
		}
		if( digitalRead( downButtonPin ) == HIGH ) {
			downPressed();
		}
	}
	
    move();
    
	switch( state ) {
		case offmark:
			//Turn off motors
			if( digitalRead(bottomStopPin) == HIGH ) {	
				state = onmark;
			}
			break;
		case onmark:
			if( digitalRead(bottomStopPin) == LOW ) {
				state = started;
			}			
			break;			
		case started:
			//Turn on motors
			if( digitalRead(bottomStopPin) == HIGH ) {
				state = onmark;
			} else if( digitalRead(topStopPin) == HIGH ) {
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
	
	if( state != previousState ) {
//		Serial.println(getState());
		previousState = state;
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

void Player::setControls(bool enabled) {
	controlsEnabled = enabled;
}

void Player::stopMotors() {
	motor->brake();
}

void Player::reset() {
	setControls(false);
	state = offmark;	
	digitalWrite(outPin, LOW);	
	move(-DEFAULT_SPEED / 2);
}

void Player::upPressed() {
	currentSpeed = DEFAULT_SPEED;
}

void Player::downPressed() {
	currentSpeed = -DEFAULT_SPEED;
}


// PRIVATE FUNCTIONS

void Player::initializePins() {
	pinMode( topStopPin, INPUT);
	pinMode( bottomStopPin, INPUT);
	pinMode( outPin, OUTPUT);
	pinMode( upButtonPin, INPUT );
	pinMode( downButtonPin, INPUT );
}

void Player::move(int newSpeed) {
	currentSpeed = newSpeed;
	move();
}

void Player::move() {
	if( digitalRead(topStopPin) == HIGH && currentSpeed > 0 ) {
		motor->brake();
		currentSpeed = 0;
	} else if( digitalRead(bottomStopPin) == HIGH && currentSpeed < 0) {
		motor->brake();
		currentSpeed = 0;
	} else if( currentSpeed != 0 ) {
		motor->drive(currentSpeed);
	} else {
		motor->brake();
	}	
}

