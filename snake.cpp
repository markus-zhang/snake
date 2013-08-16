#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Snake.h"
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include <time.h>
using namespace std;

SDL_Surface* ss_snakebit = NULL;
SDL_Surface* ss_wall = NULL;
SDL_Surface* ss_gold = NULL;
SDL_Surface* ss_score = NULL;
SDL_Surface* ss_level = NULL;
SDL_Surface* ss_scoreNum = NULL;
SDL_Surface* ss_levelNum = NULL;
string score = "SCORE";
string level = "LEVEL";
string scoreNum, levelNum;
SDL_Event event;
SDL_Color textColor = {100, 100, 100};
TTF_Font *font = NULL;
bool init()	{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)	{
		return false;
	}

	//Initialize screen
	if (screen == NULL)	{
		return false;
	}

	//Initialize font
	if (TTF_Init() != 0)	{
		return false;
	}

	SDL_WM_SetCaption("Snake v0.01", NULL);
	return true;
}

bool load_files(engine* eng)	{
	ss_snakebit = eng->engine_makeSurface(ss_snakebit, "gameResource.png");
	ss_wall = eng->engine_makeSurface(ss_snakebit, "gameResource.png");
	ss_gold = eng->engine_makeSurface(ss_snakebit, "gameResource.png");
	font = TTF_OpenFont("gameFont.ttf", 24);
	if (font == NULL)	{
		cout << TTF_GetError() << endl;
	}
	//ss_score = TTF_RenderText_Solid(font, "SCORE", textColor);
	//ss_level = TTF_RenderText_Solid(font, "LEVEL", textColor);
	ss_score = eng->engine_makeMessage(font, ss_score, score, textColor);
	ss_level = eng->engine_makeMessage(font, ss_level, level, textColor);

	assert(ss_snakebit != NULL);
	assert(ss_wall != NULL);
	assert(ss_gold != NULL);
	assert(font != NULL);
	assert(ss_score != NULL);
	assert(ss_level != NULL);
	return true;
}

bool clean()	{
	SDL_FreeSurface(ss_snakebit);
	SDL_FreeSurface(ss_wall);
	SDL_FreeSurface(ss_gold);
	SDL_FreeSurface(ss_score);
	SDL_FreeSurface(ss_level);

	//Close Font
	TTF_CloseFont(font);
	TTF_Quit();

	SDL_Quit();
	return true;
}

bool lose(game* sgame)	{
	if (sgame->game_lose())	{
		return true;
	}
	return false;
}

bool playerwin(game* sgame)	{
	if (sgame->game_win())	{
		return true;
	}
	return false;
}

string int2str(int original)	{
	stringstream ss;
	ss << original;
	return ss.str();
}

bool pause(game* sgame)	{
	if (sgame->pause == 1)	{
		return true;
	}
	return false;
}

int main(int argc, char* args[])	{
	srand(time(NULL));
	engine* snakeEngine = new engine;
	game* snakeGame = new game;
	sprites* snakeSprites = new sprites;
	snakebit* snakeBit = new snakebit;
	wall* snakeWall = new wall;
	gold* snakeGold = new gold;
	msg snakeScore, snakeLevel, snakeScoreNum, snakeLevelNum;
	sprite sprSnake;
	sprite sprWall;
	sprite sprGold;
	bool key = 1;				//Avoid two keystrokes in 1 frame
	
	snakeEngine->engine_makeScreen();
	assert(screen != NULL);
	//Initiliaze
	if (init() == false)	{
		return 1;
	}
	if (!load_files(snakeEngine))	{
		return 1;
	}
	snakeBit->sprite_id = "snakebit";
	snakeWall->sprite_id = "wall";
	snakeGold->sprite_id = "gold";

	sprSnake.sprite_set(ss_snakebit, "snakebit");
	sprWall.sprite_set(ss_wall, "wall");
	sprGold.sprite_set(ss_gold, "gold");

	snakeSprites->sprites_push(sprSnake);
	snakeSprites->sprites_push(sprWall);
	snakeSprites->sprites_push(sprGold);

	//Setup message
	snakeScore.pos.x = 100;
	snakeScore.pos.y = 50;
	snakeScore.message = ss_score;
	snakeLevel.pos.x = 200;
	snakeLevel.pos.y = 50;
	snakeLevel.message = ss_level;
	snakeScoreNum.pos.x = 160;
	snakeScoreNum.pos.y = 50;
	scoreNum = int2str(snakeGame->game_getScore());
	ss_scoreNum = snakeEngine->engine_makeMessage(font, ss_scoreNum, scoreNum, textColor);
	snakeScoreNum.message = ss_scoreNum;
	snakeLevelNum.pos.x = 260;
	snakeLevelNum.pos.y = 50;
	levelNum = int2str(snakeGame->game_getLevel());
	ss_levelNum = snakeEngine->engine_makeMessage(font, ss_levelNum, levelNum, textColor);
	snakeLevelNum.message = ss_levelNum;
	snakeGame->game_getMsg().msgList_push(snakeScore);
	snakeGame->game_getMsg().msgList_push(snakeLevel);
	snakeGame->game_getMsg().msgList_push(snakeScoreNum);
	snakeGame->game_getMsg().msgList_push(snakeLevelNum);

	bool quit = false;
	bool win = false;
	bool pause = false;

	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 255, 255));
	if (!snakeGame->game_init())	{
		return 1;
	}
	snakeGame->game_getWalls().wallList_render(*snakeSprites, *snakeEngine);
	SDL_Flip(screen);

	while (quit == false)	{
		if (lose(snakeGame))	{
			SDL_Delay(100);
			quit = true;
			break;
		}
		if (playerwin(snakeGame))	{
			cout << "Level up!" << endl;
			snakeGame->game_levelup();
			game* nextGame = new game;
			nextGame->game_setScore(snakeGame->game_getScore());
			nextGame->game_setLevel(snakeGame->game_getLevel());
			delete snakeGame;
			snakeGame = nextGame;
			snakeGame->game_init();

			snakeScore.pos.x = 100;
			snakeScore.pos.y = 50;
			snakeScore.message = ss_score;
			snakeLevel.pos.x = 200;
			snakeLevel.pos.y = 50;
			snakeLevel.message = ss_level;
			snakeScoreNum.pos.x = 160;
			snakeScoreNum.pos.y = 50;
			scoreNum = int2str(snakeGame->game_getScore());
			ss_scoreNum = snakeEngine->engine_makeMessage(font, ss_scoreNum, scoreNum, textColor);
			snakeScoreNum.message = ss_scoreNum;
			snakeLevelNum.pos.x = 260;
			snakeLevelNum.pos.y = 50;
			levelNum = int2str(snakeGame->game_getLevel());
			ss_levelNum = snakeEngine->engine_makeMessage(font, ss_levelNum, levelNum, textColor);
			snakeLevelNum.message = ss_levelNum;
			snakeGame->game_getMsg().msgList_push(snakeScore);
			snakeGame->game_getMsg().msgList_push(snakeLevel);
			snakeGame->game_getMsg().msgList_push(snakeScoreNum);
			snakeGame->game_getMsg().msgList_push(snakeLevelNum);
		}
		if (snakeGame->game_hitgold())	{
			snakeGame->game_getMsg().msgList_pop();
			snakeGame->game_getMsg().msgList_pop();
			scoreNum = int2str(snakeGame->game_getScore());
			ss_scoreNum = snakeEngine->engine_makeMessage(font, ss_scoreNum, scoreNum, textColor);
			snakeScoreNum.message = ss_scoreNum;
			levelNum = int2str(snakeGame->game_getLevel());
			ss_levelNum = snakeEngine->engine_makeMessage(font, ss_levelNum, levelNum, textColor);
			snakeLevelNum.message = ss_levelNum;
			snakeGame->game_getMsg().msgList_push(snakeScoreNum);
			snakeGame->game_getMsg().msgList_push(snakeLevelNum);

			snakeGame->game_addplayer();
		}
		//if (win == true)	{
		//	if (turn % 2 == 1)	{
		//		cout << "player1 wins" << endl;
		//		tttMessage->message = message1;
		//		tttMessage->msg_render(0, 350, *tttEngine);
		//		SDL_Flip(screen);
		//	}
		//	else	{
		//		cout << "player2 wins" << endl;
		//		tttMessage->message = message2;
		//		tttMessage->msg_render(0, 350, *tttEngine);
		//		SDL_Flip(screen);
		//	}
		//	//cout << "Player" << turn % 2 << " wins!";
		//	SDL_Delay(2000);
		//	quit = true;
		//}
		while (SDL_PollEvent(&event))	{
			if (event.type == SDL_QUIT)	{
				SDL_Delay(1000);
				quit = true;
			}

			if (event.type == SDL_KEYDOWN)	{
				switch(event.key.keysym.sym)	{
				case SDLK_UP:
					if (key != 1 || pause == true)	{
						break;
					}
					if (snakeGame->game_getDirection() % 2 == 1)	{
						break;
					}
					snakeGame->game_setDirection(1);
					key = 0;
					break;
				case SDLK_DOWN:
					if (key != 1 || pause == true)	{
						break;
					}
					if (snakeGame->game_getDirection() % 2 == 1)	{
						break;
					}
					snakeGame->game_setDirection(3);
					key = 0;
					break;
				case SDLK_LEFT:
					if (key != 1 || pause == true)	{
						break;
					}
					if (snakeGame->game_getDirection() % 2 == 0)	{	//if 2(west) or 4(east)
						break;
					}
					snakeGame->game_setDirection(2);
					key = 0;
					break;
				case SDLK_RIGHT:
					if (key != 1 || pause == true)	{
						break;
					}
					if (snakeGame->game_getDirection() % 2 == 0)	{	//if 2(west) or 4(east)
						break;
					}
					snakeGame->game_setDirection(4);
					key = 0;
					break;
				case SDLK_RETURN:
					break;
				case SDLK_ESCAPE:
					if (pause == true)	{
						pause = false;
					}
					else	{
						pause = true;
					}
					break;
				default:
					break;
				}
		}
		}
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 255, 255));
		snakeGame->game_getWalls().wallList_render(*snakeSprites, *snakeEngine);
		snakeGame->game_getGold().gold_render(*snakeSprites, *snakeEngine);
		snakeGame->game_getPlayer().snake_render(*snakeSprites, *snakeEngine);
		snakeGame->game_getMsg().msgList_render(*snakeEngine);
		SDL_Flip(screen);
		if (pause == false)	{
			snakeGame->game_moveplayer();
		}
		SDL_Delay(snakeGame->game_getSpeed());
		key = 1;			//Reset key
	}
	clean();
}
