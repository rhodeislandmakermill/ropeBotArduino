#ifndef PINS_H
#define PINS_H

//LEDS
const int INDICATOR_LED = 2;

//Race end stops
const int PLAYER_0_TOP = 14;
const int PLAYER_0_BOTTOM = 15;
const int PLAYER_1_TOP = 17;
const int PLAYER_1_BOTTOM = 18;

//Start button
const int RESET_BUTTON = 16;

//Player buttons
const int PLAYER_0_UP = 10;
const int PLAYER_0_DOWN = 11;
const int PLAYER_1_UP = 12;
const int PLAYER_1_DOWN = 13;

//Motor driver pins
const int PWMA = 8;
const int AIN1 = 7;
const int AIN2 = 6;
const int STBY = 5;
const int BIN1 = 4;
const int BIN2 = 3;
const int PWMB = 2;

//Raspberry Pi pins
const int BEGIN_IN = 53;
const int PLAYERSREADY_OUT = 51;
const int PLAYER_0_OUT = 49;
const int PLAYER_1_OUT = 47;

#endif
