//  RopeBot Due
//
//  Code for Arduino Due running game controllers for Ocean State Maker Mill Ropebot race.
//
//  The Due controls:
//  * A starter button, 
//  * Two up down controllers for players, 
//  * Two motors through Sparkfun HBridge breakout board for the bots 
//  * Communicates with Raspberry Pi which manages the race and displays results.

#include "pins.h"
#include "player.h"
#include <SparkFun_TB6612.h>

const int playerCount = 2;
Player* players[playerCount];

Motor motor1 = Motor( AIN1, AIN2, PWMA, 1, STBY );
Motor motor2 = Motor( BIN1, BIN2, PWMB, 1, STBY );

enum RaceState {
	initiating,
	playersReady,
	falseStart,
	begun,
	complete
} raceState, lastRaceState;

long startTime;

void setup() {
	players[0] = new Player(PLAYER_0_TOP, PLAYER_0_BOTTOM, PLAYER_0_OUT, PLAYER_0_UP, PLAYER_0_DOWN, &motor1 );
	players[1] = new Player(PLAYER_1_TOP, PLAYER_1_BOTTOM, PLAYER_1_OUT, PLAYER_1_UP, PLAYER_1_DOWN, &motor2 );

	pinMode( RESET_BUTTON, INPUT );
	pinMode( PLAYERSREADY_OUT, OUTPUT );
	pinMode( BEGIN_IN, INPUT );
	
	startTime = millis();
	Serial.begin(9600);

	for(int i = 0; i < 5; i++) {
		digitalWrite( PLAYERSREADY_OUT, HIGH );
		delay(20);
		digitalWrite( PLAYERSREADY_OUT, LOW );
		delay(300);
	}

	Serial.println("Start up");
}

void loop() {

	//Update player state
	bool raceBegun = true;
	if( raceState == initiating || raceState == playersReady ) {
		raceBegun = false;
	}
	
	for( int i = 0; i < playerCount; i++) {
		players[i]->updateState();
	}
	
	updateRaceState();
	debug(500);
}

void resetRace() {
	//RESET PLAYERS
	for( int i = 0; i < playerCount; i++) {  
		players[i]->reset();
	}

	//RESET RACE
	digitalWrite( PLAYERSREADY_OUT, LOW);
	raceState = initiating;
}

void updateRaceState() {
	bool allplayersReady;
	int finishedCount;
	
	switch( raceState ) {
		case initiating:
			//Move all players down until on mark
			allplayersReady = true;
			for( int i = 0; i < playerCount; i++) {
				if( players[i]->isOnMark() == false ) {
					allplayersReady = false;
					break;
				}
			}
			if( allplayersReady ) {
				raceState = playersReady;
				digitalWrite( PLAYERSREADY_OUT, HIGH );
			}
			break;
		case playersReady:
			//Enable motors
			if( digitalRead( BEGIN_IN ) == HIGH ) {
				raceState = begun;
				digitalWrite( PLAYERSREADY_OUT, LOW );
			} else {
				for( int i = 0; i < playerCount; i++) {
					if( players[i]->isOnMark() == false ) {
						raceState = falseStart;
						players[i]->setOut(HIGH);
						break;
					}
				}
			}			
			break;
		case falseStart:
			//Disable motors
			delay(2000);
			resetRace();
			break;	
		case begun:
			finishedCount = 0;
			for( int i = 0; i < playerCount; i++) {
				if( players[i]->isFinished() ) {
					players[i]->setOut(HIGH);
					finishedCount += 1;
				}				
			}
			if( finishedCount == playerCount ) {
				raceState = complete;
			}
			break;
		case complete:
			//Disable motors
			if( digitalRead( RESET_BUTTON ) == HIGH ) {
				resetRace();
			}
			break;
	}
}

void debug(long interval) {
	if( lastRaceState != raceState ) {
		switch( raceState ) {
			case initiating:
				Serial.println("Initiating");
				break;
			case playersReady:
				Serial.println("Players set");
				break;
			case falseStart:
				Serial.println("False start");
				break;
			case begun:
				Serial.println("Begun");
				break;
			case complete:
				Serial.println("Complete");
				break;
		}
	}
	lastRaceState = raceState;
}









