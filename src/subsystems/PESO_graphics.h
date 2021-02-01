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
#define _TAG_OFFSET_				(10.0)

static const SDL_Color RED		= {0xFF, 0, 0};
static const SDL_Color GREEN	= {0, 0xFF, 0};
static const SDL_Color BLUE		= {0, 0, 0xFF};
static const SDL_Color YELLOW	= {0xFF, 0xFF, 0};
static const SDL_Color WHITE	= {0xFF, 0xFF, 0xFF};
static const SDL_Color BLACK	= {0, 0, 0};

class PESO_Graphics {
private:
#pragma region DISPLAY
	SDL_Window* xyViewport		= nullptr;
	SDL_Window* xzViewport		= nullptr;
	SDL_Window* yzViewport		= nullptr;
	SDL_Window* dataViewport	= nullptr;

	SDL_Renderer* xyRenderer	= nullptr;
	SDL_Renderer* xzRenderer	= nullptr;
	SDL_Renderer* yzRenderer	= nullptr;
	SDL_Renderer* dataRenderer	= nullptr;

	SDL_Color drawColor;
	SDL_Color clearingColor;

	TTF_Font* font				= nullptr;
#pragma endregion
	

public:
	PESO_Graphics();
	~PESO_Graphics();

	void PESO_UseFont(TTF_Font* font);
	void PESO_ClearScreen();
	void PESO_ShowScreen();
	
	void PESO_DrawPointXY(const Point2d& point);
	void PESO_DrawPointXZ(const Point2d& point);
	void PESO_DrawPointYZ(const Point2d& point);

	void PESO_DrawLineSegmentXY(const Line2i& line);
	void PESO_DrawLineSegmentXZ(const Line2i& line);
	void PESO_DrawLineSegmentYZ(const Line2i& line);

	void PESO_DrawEllipseXY(const Point2d& centre, const double& radiusX, const double& radiusY);
	void PESO_DrawEllipseXZ(const Point2d& centre, const double& radiusX, const double& radiusZ);
	void PESO_DrawEllipseYZ(const Point2d& centre, const double& radiusY, const double& radiusZ);

	static SDL_Texture* PESO_CreateTextureFromSurface(SDL_Renderer* rdr, SDL_Surface* surface);
	static SDL_Texture* PESO_CreateTextureFromString(SDL_Renderer* rdr, const std::string& textureString, TTF_Font* font, SDL_Color color);
	
	void PESO_DrawSimulationData(std::shared_ptr<PESO_Object> obj);
	void PESO_DrawTag(std::shared_ptr<PESO_Object> obj);
	void PESO_DrawTagXY(const std::string& text, const double& horz, const double& vert);
	void PESO_DrawTagXZ(const std::string& text, const double& horz, const double& vert);
	void PESO_DrawTagYZ(const std::string& text, const double& horz, const double& vert);
	void PESO_DrawData(const std::string& text, const double& x, const double& y);
	void PESO_SetDrawColor(const SDL_Color& color);
	void PESO_SetClearingColor(const SDL_Color& color);
};

#endif
