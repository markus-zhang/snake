//Engine related
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "define.h"
#include <vector>
#include <string>
using namespace std;

class sprite	{
	friend class walls;
	friend class snakeList;
	friend struct gold;
	friend class sprites;
	friend class engine;
private:
	SDL_Surface* surface;
	
public:
	string sprite_id;
	const string sprite_getId()	{
		return sprite_id;
	}
	void sprite_set(SDL_Surface* newSurface, const string& id)	{
		surface = newSurface;
		sprite_id = id;
	}
	SDL_Surface* sprite_get()	{
		return surface;
	}
	void sprite_setId(const string& id)	{
		sprite_id = id;
	}
};

class sprites	{
	friend class engine;
	friend class board;
private:
	
	//TTF_Font* font;
	//vector<SDL_Surface*> messages;
public:
	vector<sprite> sprlist;
	sprites()	{
		sprlist.reserve(SPRITE_NUM);			//Maximum of 3 sprites
	}
	sprite sprites_getSprite(const string& id)	{
		for (int count = 0; count <= SPRITE_NUM; count ++)	{
			if (sprlist[count].sprite_getId() == id)	{
				return sprlist[count];
			}
		}
		return sprlist[0];
	}
	void sprites_push(const sprite& newSpirit)	{
		sprlist.push_back(newSpirit);
	}
};

class engine	{
private:
	sprites spritelist;
	//SDL_Rect clip[SPRITE_NUM];
public:
	void engine_makeScreen();
	SDL_Surface* engine_makeSurface(SDL_Surface*, const string&);
	SDL_Surface* engine_makeMessage(TTF_Font*, SDL_Surface*, string&, SDL_Color);
	void engine_applySurface(int, int, SDL_Surface*, SDL_Surface*, SDL_Rect*);
	TTF_Font* engine_loadFont(const string&, int);
	void engine_displayText(TTF_Font*, string, int, int, int, int, int, int, int, int, int);
	bool engine_clean();
	engine()	{
		clip[0].x = 0;				//First is snake
		clip[0].y = 0;
		clip[0].w = 10;
		clip[0].h = 10;
		clip[1].x = 10;				//Second is wall
		clip[1].y = 0;
		clip[1].w = 10;
		clip[1].h = 10;
		clip[2].x = 20;				//Third is gold
		clip[2].y = 0;
		clip[2].w = 10;
		clip[2].h = 10;
		clip[3].x = 30;				//Fourth is reserve
		clip[3].y = 0;
		clip[3].w = 10;
		clip[3].h = 10;
	}
};

void engine::engine_makeScreen()	{
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if (screen == NULL)	{
		//return false;
	}
	return;
}

SDL_Surface* engine::engine_makeSurface(SDL_Surface* surface, const string& picname)	{
	SDL_Surface* optimized  = NULL;

	surface = IMG_Load(picname.c_str());

	if (surface != NULL)	{
		optimized = SDL_DisplayFormat(surface);
		SDL_FreeSurface(surface);
		if (optimized != NULL)	{
			Uint32 colorkey = SDL_MapRGB(optimized->format, CKEY_R, CKEY_G, CKEY_B);
			SDL_SetColorKey(optimized, SDL_SRCCOLORKEY, colorkey);
		}
		return optimized;
	}
}

void engine::engine_applySurface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)	{
	SDL_Rect offset;

	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

TTF_Font* engine::engine_loadFont(const string& fontname, int fontsize)	{
	//spritelist.font = TTF_OpenFont(fontname.c_str(), fontsize);
	TTF_Font* font;
	font = TTF_OpenFont(fontname.c_str(), fontsize);
	if (font != NULL)	{
		return font;
	}
	//return false;
}

SDL_Surface* engine::engine_makeMessage(TTF_Font* font, SDL_Surface* message, string& text, SDL_Color textColor)	{
	message = TTF_RenderText_Solid(font, text.c_str(), textColor);
	return message;
}

void engine_displayText(TTF_Font* font, string text, int x, int y, int size, int fR, int fG, int fB, int bR, int bG, int bB)	{
	SDL_Color foreground = {fR, fG, fB};
	SDL_Color background = {bR, bG, bB};
	SDL_Surface* temp = TTF_RenderText_Shaded(font, text.c_str(), foreground, background);
}

bool engine::engine_clean()	{
	/*for (int count = 0; count <= spritelist.sprlist.size() - 1; count ++)	{
		cout << count << endl;
		SDL_FreeSurface(spritelist.sprlist[count].sprite_get());
	}*/

	//Close Font
	//TTF_CloseFont(spritelist.font);
	TTF_Quit();

	SDL_Quit();
	return true;
}
