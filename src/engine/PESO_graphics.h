/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _GRAPHICS_
#define _GRAPHICS_

#include <string>
#include <memory>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "PESO_debug.h"
#include "PESO_math.h"

#define _DEFAULT_WINDOW_POS_X_		(100)
#define _DEFAULT_WINDOW_POS_Y_		(100)
#define _DEFAULT_WINDOW_WIDTH_		(1920)
#define _DEFAULT_WINDOW_HEIGHT_		(1080)

static const SDL_Color RED		= {0xFF, 0, 0};
static const SDL_Color GREEN	= {0, 0xFF, 0};
static const SDL_Color BLUE		= {0, 0, 0xFF};
static const SDL_Color YELLOW	= {0xFF, 0xFF, 0};
static const SDL_Color WHITE	= {0xFF, 0xFF, 0xFF};
static const SDL_Color BLACK	= {0, 0, 0};

class PESO_Graphics {
private:
	SDL_Window* win;
	static SDL_Renderer* rdr;
	SDL_Color drawingColor;
	TTF_Font* font;
	Uint32 framerateAverage, frameratePrevious, framerateStart, framerateEnd;

public:
	PESO_Graphics();
	~PESO_Graphics();

	void PESO_UseFont(TTF_Font* font);
	void PESO_Clear();
	void PESO_ShowScreen();

	void PESO_DrawRectangle(const Rectangle2d& rectangle, const SDL_Color& color);
	
	void PESO_DrawPoint(const Point2d& point, const SDL_Color& color);

	void PESO_DrawLineSegment(const Line2i& line, const SDL_Color& color);
	void PESO_DrawEllipse(const Point2d& centre, const double& radiusX, const double& radiusY, const SDL_Color& color);

	static SDL_Texture* PESO_CreateTextureFromSurface(SDL_Surface* surface);
	static SDL_Texture* PESO_CreateTextureFromString(const std::string& textureString, TTF_Font* font, SDL_Color color);
	
	void PESO_DrawTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst, const double& angle, const SDL_Point* centre, SDL_RendererFlip flip);
	void PESO_DrawText(const std::string& text, const double& x, const double& y);
	void PESO_SetDrawColor(const SDL_Color& color);
	
	void PESO_SetWindowIcon(const char* fn);
	void PESO_SetWindowSize(const int& width, const int& height);
	void PESO_SetWindowTitle(const std::string& title);
	void PESO_SetVSync(bool b);
	void PESO_SetFrameStart();
	void PESO_AdjustFramerateDelay(const Uint32& delay);
	Uint32 PESO_GetAverageFramerate();
};

#endif
