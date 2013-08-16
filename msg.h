#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "define.h"
#include "engine.h";
const int MSG_NUM = 4;

struct msg	{
	SDL_Surface* message;
	coor pos;
	void msg_render(int x, int y, engine eng)	{
		eng.engine_applySurface(x, y, message, screen, NULL);
	}
};
class msgList	{
public:
	vector<msg> msgs;
	int numElement;
	msgList()	{
		msgs.reserve(MSG_NUM);
		numElement = 0;
	}
	void msgList_render(engine);
	void msgList_push(msg);
	void msgList_pop();
	void msgList_clear();
	vector<msg>& msgList_getList()	{
		return msgs;
	}
};

void msgList::msgList_render(engine eng)	{
	for (int i = 0; i <= numElement - 1; i ++)	{
		msgs[i].msg_render(msgs[i].pos.x, msgs[i].pos.y, eng);
	}
}

void msgList::msgList_push(msg newMsg)	{
	msgs.push_back(newMsg);
	numElement += 1;
}

void msgList::msgList_clear()	{
	msgs.clear();
}

void msgList::msgList_pop()	{
	msgs.pop_back();
	numElement -= 1;
}
