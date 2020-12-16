/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _GRAPHICS_
#define _GRAPHICS_

#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "PESO_math.h"
#include "PESO_physics.h"

#define _DEFAULT_WINDOW_POS_X_		(50)
#define _DEFAULT_WINDOW_POS_Y_		(50)
#define _DEFAULT_WINDOW_WIDTH_		(900)
#define _DEFAULT_WINDOW_HEIGHT_		(900)

static const SDL_Color RED		= {0xFF, 0, 0};
static const SDL_Color GREEN	= {0, 0xFF, 0};
static const SDL_Color BLUE		= {0, 0, 0xFF};
static const SDL_Color YELLOW	= {0xFF, 0xFF, 0};
static const SDL_Color WHITE	= {0xFF, 0xFF, 0xFF};
static const SDL_Color BLACK	= {0, 0, 0};

class PESO_Graphics {
private:
#pragma region DISPLAY
	SDL_Window* xyViewport;
	SDL_Window* dataViewport;

	SDL_Renderer* orbitRenderer;
	SDL_Renderer* dataRenderer;

	SDL_Color drawColor;
	SDL_Color highlightColor;
	SDL_Color clearingColor;

	TTF_Font* font;
	Uint32 framerateAverage, frameratePrevious, framerateStart, framerateEnd;
#pragma endregion

#pragma region CONTENTS
	Line2i animationTimeline;
	Point2d animationMarker;
	Point2d objectPoint;
#pragma endregion
	

public:
	PESO_Graphics();
	~PESO_Graphics();

	void PESO_UseFont(TTF_Font* font);
	void PESO_ClearScreen();
	void PESO_ShowScreen();
	
	void PESO_DrawRectangle(const Rectangle2d& rectangle);
	void PESO_DrawPoint(const Point2d& point);
	void PESO_DrawLineSegment(const Line2i& line);
	void PESO_DrawEllipse(const Point2d& centre, const double& radiusX, const double& radiusY);

	static SDL_Texture* PESO_CreateTextureFromSurface(SDL_Renderer* rdr, SDL_Surface* surface);
	static SDL_Texture* PESO_CreateTextureFromString(SDL_Renderer* rdr, const std::string& textureString, TTF_Font* font, SDL_Color color);
	
	void PESO_DrawSimulationData(std::shared_ptr<PESO_Object> obj);
	void PESO_DrawTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst, const double& angle, const SDL_Point* centre, SDL_RendererFlip flip);
	void PESO_DrawText(const std::string& text, const double& x, const double& y);
	void PESO_SetDrawColor(const SDL_Color& color);
	void PESO_SetHighlightColor(const SDL_Color& color);
	void PESO_SetClearingColor(const SDL_Color& color);
	
	Uint32 PESO_GetAverageFramerate();
};

#endif
