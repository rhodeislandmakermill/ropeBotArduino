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

enum PlayerState {
	offmark,
	set,
	started,
	halfway,
	finished
};

const int playerCount = 2;
Player* players[playerCount];

struct PlayerStruct {
	PlayerState state;
	PlayerState lastState;
	int topPin;
	int bottomPin;
	int outPin;
} playersStruct[2];

enum RaceState {
	initiating,
	playersSet,
	countDown,
	falseStart,
	begun,
	complete
} raceState, lastRaceState;

int playerStructCount;
long startTime;


void setup() {
	initializePlayers();


	players[0] = new Player(PLAYER_0_TOP, PLAYER_0_BOTTOM, PLAYER_0_OUT, PLAYER_0_UP, PLAYER_0_DOWN );
	players[1] = new Player(PLAYER_1_TOP, PLAYER_1_BOTTOM, PLAYER_1_OUT, PLAYER_1_UP, PLAYER_1_DOWN );
	
	pinMode( START_BUTTON, INPUT );
	pinMode( COUNTDOWN_OUT, OUTPUT );
	pinMode( BEGIN_IN, INPUT );
	
	startTime = millis();
	Serial.begin(9600);

	for(int i = 0; i < 5; i++) {
		digitalWrite( COUNTDOWN_OUT, HIGH );
		delay(20);
		digitalWrite( COUNTDOWN_OUT, LOW );
		delay(300);
	}

	Serial.println("Start up");
}

void loop() {
	setPlayerState();

	//Update player state
	bool raceBegun = true;
	if( raceState == initiating || raceState == playersSet ) {
		raceBegun = false;
	}
	
	for( int i = 0; i < playerCount; i++) {
		players[i]->updateState( raceBegun );
	}
	
	setRaceState();
	debug(500);
}

void initializePlayers() {	
	playersStruct[0] = {offmark, offmark, PLAYER_0_TOP, PLAYER_0_BOTTOM, PLAYER_0_OUT };
	playersStruct[1] = {offmark, offmark, PLAYER_1_TOP, PLAYER_1_BOTTOM, PLAYER_1_OUT };

	playerStructCount = sizeof(playersStruct) / sizeof(playersStruct[0]);

	for(int i = 0; i < playerStructCount; i++) {
		pinMode( playersStruct[i].topPin, INPUT);
		pinMode( playersStruct[i].bottomPin, INPUT);
		pinMode( playersStruct[i].outPin, OUTPUT);
	}
}

void resetRace() {
	//RESET PLAYERS
	for( int i = 0; i < playerStructCount; i++) {
		playersStruct[i].state = offmark;
		digitalWrite( playersStruct[i].outPin, LOW);
	}

	//RESET RACE
	digitalWrite( COUNTDOWN_OUT, LOW);
	raceState = initiating;
	lastRaceState = raceState;
}

void setPlayerState() {
	for( int i = 0; i < playerStructCount; i++) {
		switch( playersStruct[i].state ) {
			case offmark:
				if( digitalRead( playersStruct[i].bottomPin) == HIGH ) {	
					playersStruct[i].state = set;
				}
				break;
			case set:
				if( digitalRead( playersStruct[i].bottomPin) == LOW ) {
					if( raceState == initiating || raceState == playersSet ) {
						playersStruct[i].state = offmark;	
					} else {
						playersStruct[i].state = started;
					}
				}			
				break;			
			case started:
				if( digitalRead( playersStruct[i].topPin) == HIGH ) {
					playersStruct[i].state = halfway;
				}
				break;
			case halfway:
				if( digitalRead( playersStruct[i].bottomPin) == HIGH ) {
					playersStruct[i].state = finished;
				}
				break;
			case finished:
				// Not really sure if we need this one
				break;
		}
	}
}

void setRaceState() {
	bool allPlayersSet;
	int finishedCount;
	
	switch( raceState ) {
		case initiating:
			allPlayersSet = true;
			for( int i = 0; i < playerStructCount; i++) {
				if( playersStruct[i].state != set ) {
					allPlayersSet = false;
					break;
				}
			}
			if( allPlayersSet ) {
				raceState = playersSet;
			}
			break;
		case playersSet:	
			if( digitalRead( START_BUTTON ) == HIGH ) {
				digitalWrite( COUNTDOWN_OUT, HIGH );
				raceState = countDown;		
			} else {
				for( int i = 0; i < playerStructCount; i++) {
					if( playersStruct[i].state != set ) {
						raceState = initiating;
						break;
					}
				}				
			}
			break;
		case countDown:
			if( digitalRead( BEGIN_IN ) == HIGH ) {
				raceState = begun;
			} else {
				for( int i = 0; i < playerStructCount; i++) {
					if( playersStruct[i].state != set ) {
						raceState = falseStart;
						digitalWrite( playersStruct[i].outPin, HIGH );
						break;
					}
				}
			}			
			break;
		case falseStart:
			//TODO: Disable motors
			delay(2000);
			resetRace();
			raceState = initiating;
			break;	
		case begun:
			finishedCount = 0;
			for( int i = 0; i < playerStructCount; i++) {
				if( playersStruct[i].state == finished ) {
					digitalWrite( playersStruct[i].outPin, HIGH);
					finishedCount += 1;
				}				
			}
			if( finishedCount == playerStructCount ) {
				raceState = complete;
			}
			break;
		case complete:
			if( digitalRead( START_BUTTON ) == HIGH ) {
				resetRace();
				raceState = initiating;
			}
			break;
	}
}

void debug(long interval) {
	for( int i = 0; i < playerStructCount; i++) {
		if( playersStruct[i].state != playersStruct[i].lastState ) {
			Serial.print("Player ");
			Serial.print(i);
			Serial.print(" ");
			switch(  playersStruct[i].state ) {
				case offmark:
					Serial.println("offmark");
					break;
				case set:
					Serial.println("set");
					break;
				case started:
					Serial.println("started");
					break;
				case halfway:
					Serial.println("halfway");
					break;
				case finished:
					Serial.println("finished");
					break;
			}
		}
		playersStruct[i].lastState = playersStruct[i].state; 
	}
	
	if( lastRaceState != raceState ) {
		switch( raceState ) {
			case initiating:
				Serial.println("Initiating");
				break;
			case playersSet:
				Serial.println("Players set");
				break;
			case countDown:
				Serial.println("Count down");
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









