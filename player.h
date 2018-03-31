#ifndef PLAYER_H
#define PLAYER_H

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
		Player(int top, int bottom, int out, int up, int down );
		
		void updateState( bool raceBegun );

		const char* getState();
		bool isOnMark();
		bool isFinished();
		void setOut(bool value);
		void reset();
		
	private:
		int topStopPin;
		int bottomStopPin;
		int outPin;
		int upButtonPin;
		int downButtonPin;
		PlayerState state;

		void initializePins();
};

#endif
