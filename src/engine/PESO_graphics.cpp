/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#include "PESO_graphics.h"

// null out garbage memory for the renderer.
SDL_Renderer* PESO_Graphics::rdr = nullptr;

PESO_Graphics::PESO_Graphics() : framerateAverage(0), frameratePrevious(0), framerateStart(0), framerateEnd(0), drawingColor({ 0, 0, 0, 255 }) {
	win = SDL_CreateWindow(
		"PESO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		_DEFAULT_WINDOW_WIDTH_,
		_DEFAULT_WINDOW_HEIGHT_,
		SDL_WINDOW_SHOWN);

	if (win == nullptr) {
		printf("Could not create window.");
	}

	rdr = SDL_CreateRenderer(
		win,
		-1,
		SDL_RENDERER_ACCELERATED);

	if (rdr == nullptr) {
		printf("Could not create renderer.");
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("Could not init SDL_image - PNG.");
	}

	if (TTF_Init() < 0) {
		printf("Could not init SDL_ttf.");
	}
}

PESO_Graphics::~PESO_Graphics() {
	printf("Graphics shutting down...");
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyWindow(win);
	SDL_Quit();
	printf("Graphics shit down.");
}

void PESO_Graphics::PESO_DrawPoint(const Point2d& point, const SDL_Color& color) {
	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(rdr, point.x, point.y);
	SDL_SetRenderDrawColor(rdr, drawingColor.r, drawingColor.g, drawingColor.b, drawingColor.a);
}

void PESO_Graphics::PESO_DrawLineSegment(const Line2i& line, const SDL_Color& color) {
	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(rdr, line.startPoint.x, line.startPoint.y, line.endPoint.x, line.endPoint.y);
	SDL_SetRenderDrawColor(rdr, drawingColor.r, drawingColor.g, drawingColor.b, drawingColor.a);
}

void PESO_Graphics::PESO_DrawRectangle(const Rectangle2d& rectangle, const SDL_Color& color) {
	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(rdr, &rectangle.getSDLRectFrom());
	SDL_SetRenderDrawColor(rdr, drawingColor.r, drawingColor.g, drawingColor.b, drawingColor.a);
}

void PESO_Graphics::PESO_DrawEllipse(const Point2d& centre, const double& radiusX, const double& radiusY, const SDL_Color& color) {
	SDL_SetRenderDrawColor(rdr, color.r, color.g, color.b, color.a);
	for (float theta = 0.f; theta < 2 * M_PI; theta += _PI_OVER_180_) {
		int x = (int)(centre.x + radiusX * cos(theta));
		int y = (int)(centre.y + radiusY * sin(theta));
		SDL_RenderDrawPoint(rdr, x, y);
	}
	SDL_SetRenderDrawColor(rdr, drawingColor.r, drawingColor.g, drawingColor.b, drawingColor.a);
}

void PESO_Graphics::PESO_DrawTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst, const double& angle, const SDL_Point* centre, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(rdr, texture, src, dst, angle, centre, flip);
}

void PESO_Graphics::PESO_DrawText(const std::string& text, const double& x, const double& y) {
	
}

void PESO_Graphics::PESO_UseFont(TTF_Font* font) {
	if (font == nullptr) {
		printf("Could not load font.");
		return;
	}
	this->font = font;
}
