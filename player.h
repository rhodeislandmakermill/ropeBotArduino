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
	
		Player(int top, int bottom, int out, int up, int down );
		void updateState( bool raceBegun );
		

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
