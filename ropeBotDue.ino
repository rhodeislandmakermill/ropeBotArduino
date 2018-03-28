//  RopeBot Due
//
//  Code for Arduino Due running game controllers for Ocean State Maker Mill Ropebot race.
//
//  The Due controls:
//  * A starter button, 
//  * Two up down controllers for players, 
//  * Two motors through Sparkfun HBridge breakout board for the bots 
//  * Communicates with Raspberry Pi which manages the race and displays results.

// Player One button and end stop pins
const int PLAYER_0_DOWN = 10;
const int PLAYER_0_UP = 11;
const int PLAYER_0_TOP = 14;
const int PLAYER_0_BOTTOM = 15;

// Start button pin
const int START_BUTTON = 16;

// Player TWO button and end stop pins
const int PLAYER_1_DOWN = 12;
const int PLAYER_1_UP = 13;
const int PLAYER_1_TOP = 17;
const int PLAYER_1_BOTTOM = 18;

//Raspberry Pi pins
const int COUNTDOWN_OUT = 51 ;
const int GO_IN = 53;
const int PLAYER_0_OUT = 49;
const int PLAYER_1_OUT = 47;

// Motor pins
const int PWMB = 2;
const int BIN2 = 3;
const int BIN1 = 4;
const int STBY = 5;
const int AIN1 = 6;
const int AIN2 = 7;
const int PWMA = 8;

enum PlayerState {
	offmark,
	set,
	started,
	halfway,
	finished
};

struct Player {
	PlayerState state;
	PlayerState lastState;
	int topPin;
	int bottomPin;
	int outPin;
} players[2];

enum RaceState {
	initiating,
	playersSet,
	countDown,
	falseStart,
	begun,
	complete
} raceState, lastRaceState;

int playerCount;
long startTime;


void setup() {
	initializePlayers();
	pinMode( START_BUTTON, INPUT );
	pinMode( COUNTDOWN_OUT, OUTPUT );
	pinMode( GO_IN, INPUT );
	
	startTime = millis();
	Serial.begin(9600);

	for(int i = 0; i < 5; i++) {
		digitalWrite( COUNTDOWN_OUT, HIGH );
		delay(300);
		digitalWrite( COUNTDOWN_OUT, LOW );
		delay(150);
	}

	Serial.println("Start up");
}

void loop() {
	setPlayerState();
	setRaceState();
	debug(500);
}

void initializePlayers() {	
	players[0] = {offmark, offmark, PLAYER_0_TOP, PLAYER_0_BOTTOM, PLAYER_0_OUT };
	players[1] = {offmark, offmark, PLAYER_1_TOP, PLAYER_1_BOTTOM, PLAYER_1_OUT };

	playerCount = sizeof(players) / sizeof(players[0]);

	for(int i = 0; i < playerCount; i++) {
		pinMode( players[i].topPin, INPUT);
		pinMode( players[i].bottomPin, INPUT);
		pinMode( players[i].outPin, OUTPUT);
	}
}

void resetRace() {
	//RESET PLAYERS
	for( int i = 0; i < playerCount; i++) {
		players[i].state = offmark;
		digitalWrite( players[i].outPin, LOW);
	}

	//RESET RACE
	digitalWrite( COUNTDOWN_OUT, LOW);
	raceState = initiating;
	lastRaceState = raceState;
}

void setPlayerState() {
	for( int i = 0; i < playerCount; i++) {
		switch( players[i].state ) {
			case offmark:
				if( digitalRead( players[i].bottomPin) == HIGH ) {	
					players[i].state = set;
				}
				break;
			case set:
				if( digitalRead( players[i].bottomPin) == LOW ) {
					if( raceState == initiating || raceState == playersSet ) {
						players[i].state = offmark;	
					} else {
						players[i].state = started;
					}
				}			
				break;			
			case started:
				if( digitalRead( players[i].topPin) == HIGH ) {
					players[i].state = halfway;
				}
				break;
			case halfway:
				if( digitalRead( players[i].bottomPin) == HIGH ) {
					players[i].state = finished;
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
			for( int i = 0; i < playerCount; i++) {
				if( players[i].state != set ) {
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
				for( int i = 0; i < playerCount; i++) {
					if( players[i].state != set ) {
						raceState = initiating;
						break;
					}
				}				
			}
			break;
		case countDown:
			if( digitalRead( GO_IN ) == HIGH ) {
				raceState = begun;
			} else {
				for( int i = 0; i < playerCount; i++) {
					if( players[i].state != set ) {
						raceState = falseStart;
						digitalWrite( players[i].outPin, HIGH );
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
			for( int i = 0; i < playerCount; i++) {
				if( players[i].state == finished ) {
					digitalWrite( players[i].outPin, HIGH);
					finishedCount += 1;
				}				
			}
			if( finishedCount == playerCount ) {
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
	if( digitalRead(players[0].topPin) == HIGH ) {
		Serial.println("0 TOP");
	}
	for( int i = 0; i < playerCount; i++) {
		if( players[i].state != players[i].lastState ) {
			Serial.print("Player ");
			Serial.print(i);
			Serial.print(" ");
			switch(  players[i].state ) {
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
		players[i].lastState = players[i].state; 
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









