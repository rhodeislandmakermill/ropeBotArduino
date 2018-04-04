#ifndef PLAYER_H
#define PLAYER_H

#include <SparkFun_TB6612.h>
#include <Bounce2.h>

const int DEBOUNCE_TIME = 25;
const int HIT_RESET_TIME = 1000;
const int ACCELERATION_FACTOR = 10;
const int FRICTION = 70;

class Player {
	public:
		enum PlayerState {
			offmark,
			onmark,
			started,
			halfway,
			finished
		};

		const char* stateNames[5] = {"offmark","onmark","started","halfway","finished"};

		Player();
		Player(int top, int bottom, int out, int up, int down, Motor* motor);
		
		void updateState();

		const char* getState();
		bool isOnMark();
		bool isFinished();
		void setOut(bool value);
		void setControls(bool enabled);
		void stopMotors();
		void reset();
		void upPressed();
		void downPressed();

	private:
		//Pins
		int topStopPin;
		int bottomStopPin;
		int outPin;
		int upButtonPin;
		int downButtonPin;

		//Motor and controls
		Motor* motor;	
		int currentSpeed;
		bool controlsEnabled;
		int upHits;
		int downHits;
		Bounce up;
		Bounce down;
		
		struct Timer {
			unsigned long start;
			unsigned long duration;
		} hitTimer;

		//State
		PlayerState state, previousState;
		
		void initialize(bool pinsSet = true);
		void move(int newSpeed);
		void move();
};

#endif
