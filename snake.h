#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>
#include <iostream>
#include "readfile.h"
#include "msg.h"
using namespace std;

struct snakebit	{
public:
	coor pos;
	string sprite_id;
	snakebit():	sprite_id("snakebit")	{}
};

class snake	{
	friend class game;
private:
	vector<snakebit> snakeList;
	int numElement;
public:
	//Constructor
	snake()	{
		snakeList.reserve(SNAKE_LENGTH_MAX);
		numElement = 0;
	}
	//Utilities
	//Copy the pos of the ith bit to (i+1)th
	bool snake_copy();
	//Modify the headbit
	bool snake_mod(const coor&);
	//Push a snakebit to the end
	bool snake_push(snakebit);
	//Pop the last snakebit
	bool snake_pop();
	//Reset the snake
	void snake_reset();
	//Test collision with self
	bool snake_hitself();
	//Render
	void snake_render(sprites, engine);
	//Debug
	void snake_debug()	{
		//cout << snakeList[snakeList.size() - 1].pos.x << " " << snakeList[snakeList.size() - 1].pos.y << endl;
	}
	//Get snakeList
	vector<snakebit> snake_getlist()	{
		return snakeList;
	}
};
bool snake::snake_copy()	{
	if (snakeList.size() < 2)	{
		return false;
	}
	//cout << "Copy " << snakeList.size();
	for (int i = snakeList.size() - 1; i >= 1; i --)	{
		snakeList[i].pos = snakeList[i - 1].pos;
	}
	return true;
}

bool snake::snake_mod(const coor& mod)	{
	if (snakeList.size() < 1)	{
		return false;
	}
	snakeList[0].pos.x += mod.x;
	snakeList[0].pos.y += mod.y;
	return true;
}

bool snake::snake_push(snakebit bit)	{
	if (numElement >= SNAKE_LENGTH_MAX)	{
		return false;
	}
	snakeList.push_back(bit);
	numElement += 1;
	return true;
}

void snake::snake_reset()	{
	//for (int i = 0; i <= snakeList.size() - 1; i ++)	{
	//	snakeList.pop_back();
	//}
	snakeList.clear();
	snakeList.reserve(SNAKE_LENGTH_MIN);
	cout << snakeList.size() << endl;
}

bool snake::snake_pop()	{
	if (numElement <= SNAKE_LENGTH_MIN)	{
		return false;
	}
	snakeList.pop_back();
	numElement -= 1;
	return true;
}

bool snake::snake_hitself()	{
	if (numElement <= 1)	{
		return false;
	}
	for (int i = 1; i <= snakeList.size() - 1; i ++)	{
		if (snakeList[i].pos.x == snakeList[0].pos.x && snakeList[i].pos.y == snakeList[0].pos.y)	{
			return true;
		}
	}
	return false;
}

void snake::snake_render(sprites sprs, engine eng)	{
	if (snakeList.size() == 0)	{
		return;
	}
	for (int count = 0; count <= snakeList.size() - 1; count ++)	{
		sprite spr = sprs.sprites_getSprite(snakeList[count].sprite_id);
		eng.engine_applySurface(snakeList[count].pos.x, snakeList[count].pos.y, spr.sprite_get(), screen, &clip[0]);
	}
	//cout << "Render " << snakeList.size() << endl;
}

struct wall	{
	coor pos;
	string sprite_id;
};

class wallList	{
public:
	vector<wall> walls;
	int numElement;
	wallList()	{
		walls.reserve(WALL_NUM_MAX);
		numElement = 0;
	}
	//Utilities
	//Push to the end
	bool wallList_push(int x, int y, const string id);
	void wallList_render(sprites sprs, engine eng)	{
		if (numElement <= 0)	{
			return;
		}
		for (int count = 0; count <= numElement - 1; count ++)	{
			sprite spr = sprs.sprites_getSprite(walls[count].sprite_id);
			eng.engine_applySurface(walls[count].pos.x, walls[count].pos.y, spr.sprite_get(), screen, &clip[1]);
		}
	}
	void wallList_reset()	{
		walls.clear();
		walls.reserve(WALL_NUM_MAX);
	}
};

bool wallList::wallList_push(int x, int y, const string id)	{
	wall* temp = new wall;
	temp->pos.x = x;
	temp->pos.y = y;
	temp->sprite_id = id;
	walls.push_back(*temp);
	numElement += 1;
	return true;
}

struct gold	{
	coor pos;
	string sprite_id;
	gold(): sprite_id ("gold")	{}
	void gold_render(sprites sprs, engine eng)	{
		sprite spr = sprs.sprites_getSprite(sprite_id);
		eng.engine_applySurface(pos.x, pos.y, spr.sprite_get(), screen, &clip[2]);
	}
};

class game	{
	friend class snakeList;
	friend class engine;
private:
	wallList walls;
	msgList msgall;
	snake player;
	gold goldcoin;
	lines wallpos;
	lines goldpos;
	int level;					//initial at 1
	int score;					//100 for each gold
	int numGold;				//initially at 10, increases 2 per level
	int currentGold;
	int speed;					//simply a SDL_Delay() var
	int direction;				//North = 1, South = 3, etc.
public:
	int pause;
	game()	{
		msgall.msgs.reserve(4);
		level = 1;
		score = 0;
		numGold = 10;
		currentGold = 0;
		speed = 250;			//200ms delay
		direction = 1;			//North
		pause = 0;
	}
	//Initiate player
	bool game_init();
	wallList game_getWalls()	{
		return walls;
	}
	snake game_getPlayer()	{
		return player;
	}
	int game_getSpeed()	{
		return speed;
	}
	int game_getDirection()	{
		return direction;
	}
	void game_setDirection(const int& dir)	{
		if (dir >= 1 && dir <= 4)	{
			direction = dir;
		}
	}
	gold game_getGold()	{
		return goldcoin;
	}
	int game_getNumGold()	{
		return numGold;
	}
	msgList& game_getMsg()	{
		//cout << msgall.msgList_getList().size();
		return msgall;
	}
	int game_getScore()	{
		return score;
	}
	void game_setScore(int newScore)	{
		score = newScore;
	}
	int game_getLevel()	{
		return level;
	}
	void game_setLevel(int newLevel)	{
		level = newLevel;
	}
	void game_makegold();
	bool game_showwall();
	bool game_hitwall();
	bool game_hitself();
	bool game_lose();
	void game_reset();
	bool game_win();
	void game_levelup();
	bool game_hitgold();
	void game_moveplayer();
	void game_addplayer();			//Add a snakebit after taking a coin
};

void game::game_makegold()	{
	if (numGold <= 0)	{
		return;
	}
	goldcoin.pos.x = goldpos.lines_getlist()[currentGold].firstNum;
	goldcoin.pos.y = goldpos.lines_getlist()[currentGold].secondNum;
	currentGold += 1;
}

void game::game_moveplayer()	{
	if (!player.snake_copy())	{
		return;
	}
	if (direction == 1)	{		//North
		coor temp;
		temp.x = 0;
		temp.y = -2 * SNAKEBIT_HALF_LENGTH;
		player.snake_mod(temp);
	}
	if (direction == 2)	{		//West
		coor temp;
		temp.x = -2 * SNAKEBIT_HALF_LENGTH;
		temp.y = 0;
		player.snake_mod(temp);
	}
	if (direction == 3)	{		//North
		coor temp;
		temp.x = 0;
		temp.y = 2 * SNAKEBIT_HALF_LENGTH;
		player.snake_mod(temp);
	}
	if (direction == 4)	{		//East
		coor temp;
		temp.x = 2 * SNAKEBIT_HALF_LENGTH;
		temp.y = 0;
		player.snake_mod(temp);
	}
}

void game::game_reset()	{
	player.snake_reset();
	walls.wallList_reset();
}

bool game::game_init()	{
	//First we initiate the snake
	for (int i = 0; i <= SNAKE_LENGTH_MIN - 1; i++)	{
		cout << i << " ";
		snakebit temp;
		//The initial snake is vertical
		temp.pos.x = SCREEN_WIDTH / 2;
		//Every bit is 10 pixels away from the last
		temp.pos.y = SCREEN_HEIGHT / 2 + i * SNAKEBIT_HALF_LENGTH * 2;
		temp.sprite_id = "snakebit";
		player.snake_push(temp);
	}
	//Next we initiate the wall
	stringstream sswall;
	string wall = "wall";
	sswall << wall << level;
	cout << sswall.str() << endl;
	wallpos.lines_read(sswall.str());
	for (int i = 0; i <= wallpos.lines_getNum() - 1; i ++)	{
		cout << i << " ";
		walls.wallList_push(wallpos.lines_getlist()[i].firstNum, wallpos.lines_getlist()[i].secondNum , "wall");
	}
	//Next we initiate gold coin
	stringstream ssgold;
	string gold = "gold";
	ssgold << gold << level;
	cout << ssgold.str() << endl;
	goldpos.lines_read(ssgold.str());
	game_makegold();
	numGold = 10;			//Must reset for this level
	//Message
	msgall.msgList_getList().reserve(4);
	return true;
}

//This is a debug function
bool game::game_showwall()	{
	if (walls.numElement <= 0)	{
		return false;
	}
	for (int i = 0; i <= walls.numElement - 1; i ++)	{
	}
	return true;
}

bool game::game_hitwall()	{
	if (walls.numElement <= 0)	{
		return false;
	}
	for (int i = 0; i <= walls.walls.size() - 1; i ++)	{
		if (walls.walls[i].pos.x == player.snakeList[0].pos.x && walls.walls[i].pos.y == player.snakeList[0].pos.y)	{
			return true;
		}
	}
	return false;
}

bool game::game_hitself()	{
	if (player.numElement <= 1)	{
		return false;
	}
	for (int i = 1; i <= player.snake_getlist().size() - 1; i ++)	{
		if ( player.snake_getlist()[i].pos.x == player.snake_getlist()[0].pos.x && player.snake_getlist()[i].pos.y == player.snake_getlist()[0].pos.y)	{
			return true;
		}
	}
	return false;
}

bool game::game_lose()	{
	if (game_hitself() || game_hitwall())	{
		return true;
	}
	return false;
}

bool game::game_win()	{
	if (numGold == 0)	{
		return true;
	}
	return false;
}

void game::game_levelup()	{
	level += 1;
	score = 0;
}

bool game::game_hitgold()	{
	if (player.snake_getlist()[0].pos.x == goldcoin.pos.x && player.snake_getlist()[0].pos.y == goldcoin.pos.y)	{
		score += 100;
		numGold -= 1;
		game_makegold();
		return true;
	}
	return false;
}

void game::game_addplayer()	{
	player.snake_getlist().resize(player.snake_getlist().size() + 1);
	if (direction == 1)	{
		snakebit temp;
		temp.sprite_id = "snakebit";
		temp.pos.x = player.snake_getlist()[player.snake_getlist().size() - 1].pos.x;
		temp.pos.y = player.snake_getlist()[player.snake_getlist().size() - 1].pos.y + 1;
		player.snake_push(temp);
	}
	if (direction == 3)	{
		snakebit temp;
		temp.sprite_id = "snakebit";
		temp.pos.x = player.snake_getlist()[player.snake_getlist().size() - 1].pos.x;
		temp.pos.y = player.snake_getlist()[player.snake_getlist().size() - 1].pos.y - 1;
		player.snake_push(temp);
	}
	if (direction == 2)	{
		snakebit temp;
		temp.sprite_id = "snakebit";
		temp.pos.x = player.snake_getlist()[player.snake_getlist().size() - 1].pos.x + 1;
		temp.pos.y = player.snake_getlist()[player.snake_getlist().size() - 1].pos.y;
		player.snake_push(temp);
	}
	if (direction == 4)	{
		snakebit temp;
		temp.sprite_id = "snakebit";
		temp.pos.x = player.snake_getlist()[player.snake_getlist().size() - 1].pos.x - 1;
		temp.pos.y = player.snake_getlist()[player.snake_getlist().size() - 1].pos.y;
		player.snake_push(temp);
	}
}
