/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#include "PESO_graphics.h"

PESO_Graphics::PESO_Graphics() : framerateAverage(0), frameratePrevious(0), framerateStart(0), framerateEnd(0), drawColor({ WHITE }), highlightColor({ RED }), clearingColor({ BLACK }) {
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("Could not init SDL_image - PNG.");
	}

	if (TTF_Init() < 0) {
		printf("Could not init SDL_ttf.");
	}
	PESO_UseFont(TTF_OpenFont("res/fonts/arial.ttf", 15));
	
	xyViewport = SDL_CreateWindow(
		"PESO Simulation Visual",
		_DEFAULT_WINDOW_POS_X_,
		_DEFAULT_WINDOW_POS_Y_,
		_DEFAULT_WINDOW_WIDTH_,
		_DEFAULT_WINDOW_HEIGHT_,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	orbitRenderer = SDL_CreateRenderer(
		xyViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);

	dataViewport = SDL_CreateWindow(
		"PESO Simulation Data",
		(_DEFAULT_WINDOW_POS_X_ + 1100),
		_DEFAULT_WINDOW_POS_Y_,
		_DEFAULT_WINDOW_WIDTH_ / 2,
		_DEFAULT_WINDOW_HEIGHT_ / 2,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	dataRenderer = SDL_CreateRenderer(
		dataViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);
}

PESO_Graphics::~PESO_Graphics() {
	printf("Graphics shutting down...");
	IMG_Quit();
	TTF_Quit();
	SDL_DestroyRenderer(orbitRenderer);
	SDL_DestroyRenderer(dataRenderer);
	SDL_DestroyWindow(xyViewport);
	SDL_DestroyWindow(dataViewport);
	SDL_Quit();
	printf("Graphics shut down.");
}

void PESO_Graphics::PESO_DrawPoint(const Point2d& point) {
	SDL_SetRenderDrawColor(orbitRenderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
	SDL_RenderDrawPoint(orbitRenderer, point.x, point.y);
}

void PESO_Graphics::PESO_DrawLineSegment(const Line2i& line) {
	SDL_SetRenderDrawColor(orbitRenderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
	SDL_RenderDrawLine(orbitRenderer, line.startPoint.x, line.startPoint.y, line.endPoint.x, line.endPoint.y);
}

void PESO_Graphics::PESO_DrawRectangle(const Rectangle2d& rectangle) {
	SDL_SetRenderDrawColor(orbitRenderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
	SDL_RenderDrawRect(orbitRenderer, &rectangle.getSDLRectFrom());
}

void PESO_Graphics::PESO_DrawEllipse(const Point2d& centre, const double& radiusX, const double& radiusY) {
	SDL_SetRenderDrawColor(orbitRenderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
	for (float theta = 0.f; theta < 2 * M_PI; theta += _PI_OVER_180_) {
		int x = (int)(centre.x + radiusX * cos(theta));
		int y = (int)(centre.y + radiusY * sin(theta));
		SDL_RenderDrawPoint(orbitRenderer, x, y);
	}
}

void PESO_Graphics::PESO_DrawTexture(SDL_Texture* texture, SDL_Rect* src, SDL_Rect* dst, const double& angle, const SDL_Point* centre, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(orbitRenderer, texture, src, dst, angle, centre, flip);
}

void PESO_Graphics::PESO_DrawText(const std::string& text, const double& x, const double& y) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), drawColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(dataRenderer, textSurface);

	int width;
	int height;
	SDL_QueryTexture(textTexture, NULL, NULL, &width, &height);
	SDL_Rect dst = {
		x,
		y,
		width,
		height
	};
	SDL_RenderCopy(dataRenderer, textTexture, NULL, &dst);
}

void PESO_Graphics::PESO_DrawSimulationData(std::shared_ptr<PESO_Object> obj) {
	int width;
	int height;
	SDL_GetWindowSize(xyViewport, &width, &height);

	PESO_DrawText("Object Tag:", 10.0 , 10.0);
	PESO_DrawText(obj->getTag(), 150.0 , 10.0);
#pragma region TRANSFORM
	PESO_DrawText("Position:", 10.0 , 40.0);	
	PESO_DrawText(std::to_string(obj->getPosition().x - width/2), 150.0 , 40.0);
	PESO_DrawText(std::to_string(obj->getPosition().y - height/2), 250.0, 40.0);
	PESO_DrawText(std::to_string(obj->getPosition().z), 350.0 , 40.0);
	PESO_DrawText("Rotation:", 10.0 , 70.0);	
	PESO_DrawText(std::to_string(obj->getRotation().x), 150.0 , 70.0);
	PESO_DrawText(std::to_string(obj->getRotation().y), 250.0 , 70.0);
	PESO_DrawText(std::to_string(obj->getRotation().z), 350.0 , 70.0);
#pragma endregion
#pragma region DYNAMICS
	PESO_DrawText("Linear Velocity:", 10.0, 110.0);
	PESO_DrawText(std::to_string(obj->getLinVelocity().x), 150.0, 110.0);
	PESO_DrawText(std::to_string(obj->getLinVelocity().y), 250.0, 110.0);
	PESO_DrawText(std::to_string(obj->getLinVelocity().z), 350.0, 110.0);
	PESO_DrawText("Linear Acceleration:", 10.0, 150.0);
	PESO_DrawText(std::to_string(obj->getLinAcceleration().x), 150.0, 150.0);
	PESO_DrawText(std::to_string(obj->getLinAcceleration().y), 250.0, 150.0);
	PESO_DrawText(std::to_string(obj->getLinAcceleration().z), 350.0, 150.0);
#pragma endregion
}

void PESO_Graphics::PESO_ClearScreen() {
	SDL_SetRenderDrawColor(orbitRenderer, clearingColor.r, clearingColor.g, clearingColor.b, clearingColor.a);
	SDL_SetRenderDrawColor(dataRenderer, clearingColor.r, clearingColor.g, clearingColor.b, clearingColor.a);
	SDL_RenderClear(orbitRenderer);
	SDL_RenderClear(dataRenderer);
}

void PESO_Graphics::PESO_ShowScreen() {
	SDL_RenderPresent(orbitRenderer);
	SDL_RenderPresent(dataRenderer);
}

void PESO_Graphics::PESO_UseFont(TTF_Font* font) {
	if (font == nullptr) {
		printf("Could not load font.");
		return;
	}
	this->font = font;
}

void PESO_Graphics::PESO_SetDrawColor(const SDL_Color& color) {
	this->drawColor = color;
}

void PESO_Graphics::PESO_SetHighlightColor(const SDL_Color& color) {
	this->highlightColor = color;
}

void PESO_Graphics::PESO_SetClearingColor(const SDL_Color& color) {
	this->clearingColor = color;
}

SDL_Texture* PESO_Graphics::PESO_CreateTextureFromSurface(SDL_Renderer* rdr, SDL_Surface* surface) {
	SDL_Texture* texture = SDL_CreateTextureFromSurface(rdr, surface);
	if (texture == nullptr) {
		std::cout << "Could not create texture from provided surface.";
	}
	return texture;
}

SDL_Texture* PESO_Graphics::PESO_CreateTextureFromString(SDL_Renderer* rdr, const std::string& textureString, TTF_Font* font, SDL_Color color) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureString.c_str(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rdr, textSurface);
	if (textTexture == nullptr) {
		std::cout << "Could not create texture from provided string.";
	}
	return textTexture;
}

Uint32 PESO_Graphics::PESO_GetAverageFramerate() {
	return this->framerateAverage;
};
