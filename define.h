#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include "SDL.h"
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 500;
const int SCREEN_BPP = 24;
const int SPRITE_NUM = 4;
const int CKEY_R = 255;
const int CKEY_G = 255;
const int CKEY_B = 255;

const int SNAKE_LENGTH_MAX = 20;
const int SNAKE_LENGTH_MIN = 10;
const int SNAKEBIT_HALF_LENGTH = 5;		//10*10 for a snake bit
const int WALL_HALF_LENGTH = 5;
const int GOLD_NUM_INITIAL = 10;
const int WALL_NUM_MAX = 100;

SDL_Surface* screen = NULL;
SDL_Rect clip[SPRITE_NUM];

struct coor	{
public:
	int x;
	int y;
	coor(): x(0), y(0)	{}
};
#endif
