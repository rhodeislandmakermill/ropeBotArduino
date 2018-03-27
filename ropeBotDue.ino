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
const int PLAYER_ONE_DOWN = 10;
const int PLAYER_ONE_UP = 11;
const int PLAYER_ONE_TOP = 12;
const int PLAYER_ONE_BOTTOM = 13;

// Start button pin
const int START_BUTTON = 14;

// Player TWO button and end stop pins
const int PLAYER_TWO_DOWN = 15;
const int PLAYER_TWO_UP = 16;
const int PLAYER_TWO_TOP = 17;
const int PLAYER_TWO_BOTTOM = 18;

// Motor pins
const int PWMB = 2;
const int BIN2 = 3;
const int BIN1 = 4;
const int STBY = 5;
const int AIN1 = 6;
const int AIN2 = 7;
const int PWMA = 8;

//Raspberry Pi pins
const int PLAYER1 = 47;
const int PLAYER2 = 49;
const int START = 51;
const int RESET = 53;

enum RaceState {
	initiating,
	begun,
	complete
};

enum PlayerState {
	waiting,
	started,
	halfway,
	finished
};

volatile RaceState raceState;
volatile PlayerState playerOneState;
volatile PlayerState playerTwoState;
volatile int winner;

void setup() {


}

void loop() {

}

void initializeRace() {
	playerOneState = waiting;
	playerTwoState = waiting;
	raceState = begun;
}





