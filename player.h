#ifndef PLAYER_H
#define PLAYER_H

#include <SparkFun_TB6612.h>

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

		//Motor
		Motor* motor;	
		int currentSpeed;
		bool controlsEnabled;

		//State
		PlayerState state, previousState;
		
		void initialize(bool pinsSet = true);
		void move(int newSpeed);
		void move();
};

#endif
