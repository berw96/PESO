/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>

	Made with C++ and Simple DirectMedia Layer (SDL) Version 2.0, under the
	GNU General Public Licence (GPL) and the ZLib Licence.
*/

#include "PESO_graphics.h"

PESO_Graphics::PESO_Graphics() : drawColor({ WHITE }), clearingColor({ BLACK }) {
	if (TTF_Init() < 0) {
		printf("Could not init SDL_ttf.");
	}
	else {
		PESO_UseFont(TTF_OpenFont("res/fonts/arial.ttf", 15));
	}

#pragma region XY
	xyViewport = SDL_CreateWindow(
		"PESO Simulation XY (SIDE VIEW)",
		_DEFAULT_WINDOW_POS_X_,
		_DEFAULT_WINDOW_POS_Y_,
		_DEFAULT_WINDOW_WIDTH_,
		_DEFAULT_WINDOW_HEIGHT_,
		SDL_WINDOW_SHOWN
	);

	xyRenderer = SDL_CreateRenderer(
		xyViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);
#pragma endregion

#pragma region XZ
	xzViewport = SDL_CreateWindow(
		"PESO Simulation XZ (TOP-DOWN VIEW)",
		_DEFAULT_WINDOW_POS_X_,
		_DEFAULT_WINDOW_POS_Y_ + _DEFAULT_WINDOW_HEIGHT_,
		_DEFAULT_WINDOW_WIDTH_,
		_DEFAULT_WINDOW_HEIGHT_,
		SDL_WINDOW_SHOWN
	);

	xzRenderer = SDL_CreateRenderer(
		xzViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);
#pragma endregion

#pragma region YZ
	yzViewport = SDL_CreateWindow(
		"PESO Simulation YZ (FRONT VIEW)",
		_DEFAULT_WINDOW_POS_X_ + _DEFAULT_WINDOW_HEIGHT_,
		_DEFAULT_WINDOW_POS_Y_,
		_DEFAULT_WINDOW_WIDTH_,
		_DEFAULT_WINDOW_HEIGHT_,
		SDL_WINDOW_SHOWN
	);

	yzRenderer = SDL_CreateRenderer(
		yzViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);
#pragma endregion

#pragma region DATA
	dataViewport = SDL_CreateWindow(
		"PESO Simulation Data",
		_DEFAULT_WINDOW_POS_X_ + (_DEFAULT_WINDOW_WIDTH_ * 2),
		_DEFAULT_WINDOW_POS_Y_,
		_DEFAULT_WINDOW_WIDTH_,
		_DEFAULT_WINDOW_HEIGHT_,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	dataRenderer = SDL_CreateRenderer(
		dataViewport,
		-1,
		SDL_RENDERER_ACCELERATED
	);
#pragma endregion
}

PESO_Graphics::~PESO_Graphics() {
	printf("PESO_Graphics shutting down...\n");
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyRenderer(xyRenderer);
	SDL_DestroyRenderer(xzRenderer);
	SDL_DestroyRenderer(yzRenderer);
	SDL_DestroyRenderer(dataRenderer);
	SDL_DestroyWindow(xyViewport);
	SDL_DestroyWindow(xzViewport);
	SDL_DestroyWindow(yzViewport);
	SDL_DestroyWindow(dataViewport);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	printf("PESO_Graphics shut down.\n");
}

void PESO_Graphics::PESO_DrawPointXY(const Point2d& point) {
	SDL_RenderDrawPoint(xyRenderer, point.horizontal, point.vertical);
}

void PESO_Graphics::PESO_DrawPointXZ(const Point2d& point) {
	SDL_RenderDrawPoint(xzRenderer, point.horizontal, point.vertical);
}

void PESO_Graphics::PESO_DrawPointYZ(const Point2d& point) {
	SDL_RenderDrawPoint(yzRenderer, point.horizontal, point.vertical);
}

void PESO_Graphics::PESO_DrawLineSegmentXY(const Line2i& line) {
	SDL_RenderDrawLine(
		xyRenderer,
		line.startPoint.horizontal,
		line.startPoint.vertical,
		line.endPoint.horizontal,
		line.endPoint.vertical
	);
}

void PESO_Graphics::PESO_DrawLineSegmentXZ(const Line2i& line) {
	SDL_RenderDrawLine(
		xzRenderer,
		line.startPoint.horizontal,
		line.startPoint.vertical,
		line.endPoint.horizontal,
		line.endPoint.vertical
	);
}

void PESO_Graphics::PESO_DrawLineSegmentYZ(const Line2i& line) {
	SDL_RenderDrawLine(
		yzRenderer,
		line.startPoint.horizontal,
		line.startPoint.vertical,
		line.endPoint.horizontal,
		line.endPoint.vertical
	);
}

void PESO_Graphics::PESO_DrawEllipseXY(const Point2d& centre, const double& radiusX, const double& radiusY) {
	SDL_SetRenderDrawColor(xyRenderer, 0xFF, 0x00, 0x00, 0xFF);
	for (float theta = 0.f; theta < 2 * M_PI; theta += _PI_OVER_180_) {
		int width = (int)(centre.horizontal + radiusX * cos(theta));
		int height = (int)(centre.vertical + radiusY * sin(theta));
		SDL_RenderDrawPoint(xyRenderer, width, height);
	}
}

void PESO_Graphics::PESO_DrawEllipseXZ(const Point2d& centre, const double& radiusX, const double& radiusZ) {
	SDL_SetRenderDrawColor(xzRenderer, 0x00, 0xFF, 0x00, 0xFF);
	for (float theta = 0.f; theta < 2 * M_PI; theta += _PI_OVER_180_) {
		int width = (int)(centre.horizontal + radiusX * cos(theta));
		int height = (int)(centre.vertical + radiusZ * sin(theta));
		SDL_RenderDrawPoint(xzRenderer, width, height);
	}
}

void PESO_Graphics::PESO_DrawEllipseYZ(const Point2d& centre, const double& radiusY, const double& radiusZ) {
	SDL_SetRenderDrawColor(yzRenderer, 0x00, 0x00, 0xFF, 0xFF);
	for (float theta = 0.f; theta < 2 * M_PI; theta += _PI_OVER_180_) {
		int width = (int)(centre.horizontal + radiusY * cos(theta));
		int height = (int)(centre.vertical + radiusZ * sin(theta));
		SDL_RenderDrawPoint(yzRenderer, width, height);
	}
}

void PESO_Graphics::PESO_DrawText(const std::string& text, const double& x, const double& y) {
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), drawColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(dataRenderer, textSurface);
	
	int width;
	int height;

	SDL_QueryTexture(textTexture, NULL, NULL, &width, &height);

	SDL_Rect dst = {
		(int)x,
		(int)y,
		width,
		height
	};

	SDL_RenderCopy(dataRenderer, textTexture, NULL, &dst);
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
}

void PESO_Graphics::PESO_DrawTagXY(const std::string& text, const double& horz, const double& vert) {
	SDL_Surface* textSurfaceXY = TTF_RenderText_Blended(font, text.c_str(), drawColor);
	SDL_Texture* textTextureXY = SDL_CreateTextureFromSurface(xyRenderer, textSurfaceXY);
	
	int widthXY, heightXY;
	
	SDL_QueryTexture(textTextureXY, NULL, NULL, &widthXY, &heightXY);
	
	SDL_Rect dstXY = {
		(int)horz,
		(int)vert,
		widthXY,
		heightXY
	};
	
	SDL_RenderCopy(xyRenderer, textTextureXY, NULL, &dstXY);
	SDL_DestroyTexture(textTextureXY);
	SDL_FreeSurface(textSurfaceXY);
}

void PESO_Graphics::PESO_DrawTagXZ(const std::string& text, const double& horz, const double& vert) {
	SDL_Surface* textSurfaceXZ = TTF_RenderText_Blended(font, text.c_str(), drawColor);
	SDL_Texture* textTextureXZ = SDL_CreateTextureFromSurface(xzRenderer, textSurfaceXZ);

	int widthXZ, heightXZ;

	SDL_QueryTexture(textTextureXZ, NULL, NULL, &widthXZ, &heightXZ);

	SDL_Rect dstXZ = {
		(int)horz,
		(int)vert,
		widthXZ,
		heightXZ
	};

	SDL_RenderCopy(xzRenderer, textTextureXZ, NULL, &dstXZ);
	SDL_DestroyTexture(textTextureXZ);
	SDL_FreeSurface(textSurfaceXZ);
}

void PESO_Graphics::PESO_DrawTagYZ(const std::string& text, const double& horz, const double& vert) {
	SDL_Surface* textSurfaceYZ = TTF_RenderText_Blended(font, text.c_str(), drawColor);
	SDL_Texture* textTextureYZ = SDL_CreateTextureFromSurface(yzRenderer, textSurfaceYZ);

	int widthYZ, heightYZ;

	SDL_QueryTexture(textTextureYZ, NULL, NULL, &widthYZ, &heightYZ);

	SDL_Rect dstYZ = {
		(int)horz,
		(int)vert,
		widthYZ,
		heightYZ
	};

	SDL_RenderCopy(yzRenderer, textTextureYZ, NULL, &dstYZ);
	SDL_DestroyTexture(textTextureYZ);
	SDL_FreeSurface(textSurfaceYZ);
}

void PESO_Graphics::PESO_DrawTag(std::shared_ptr<PESO_Object> obj) {
	PESO_DrawTagXY(obj->getTag(), obj->getPosition().x + _TAG_OFFSET_ + obj->getRadius(), obj->getPosition().y);
	PESO_DrawTagXZ(obj->getTag(), obj->getPosition().x + _TAG_OFFSET_ + obj->getRadius(), obj->getPosition().z);
	PESO_DrawTagYZ(obj->getTag(), obj->getPosition().z + _TAG_OFFSET_ + obj->getRadius(), obj->getPosition().y);
}

void PESO_Graphics::PESO_DrawSimulationData(std::shared_ptr<PESO_Object> obj) {
	PESO_DrawText("Object Tag:", 10.0 , 10.0);
	PESO_DrawText(obj->getTag(), 150.0 , 10.0);
#pragma region TRANSFORM
	PESO_DrawText("Position:", 10.0 , 40.0);	
	PESO_DrawText(std::to_string(obj->getPosition().x), 150.0 , 40.0);
	PESO_DrawText(std::to_string(obj->getPosition().y), 250.0, 40.0);
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

void PESO_Graphics::PESO_DrawObjectGraphics(PESO_Physics& physics) {
	for (auto obj : physics.PESO_GetSessionObjects()) {
		Point2d ObjectPointXY = {
			obj->getPosition().x,
			obj->getPosition().y
		};

		Point2d ObjectPointXZ = {
			obj->getPosition().x,
			obj->getPosition().z
		};

		Point2d ObjectPointYZ = {
			obj->getPosition().z,
			obj->getPosition().y
		};

		PESO_DrawTag(obj);
		PESO_DrawEllipseXY(ObjectPointXY, obj->getRadius(), obj->getRadius());
		PESO_DrawEllipseXZ(ObjectPointXZ, obj->getRadius(), obj->getRadius());
		PESO_DrawEllipseYZ(ObjectPointYZ, obj->getRadius(), obj->getRadius());
	}
}

void PESO_Graphics::PESO_ClearScreen() {
	SDL_SetRenderDrawColor(xyRenderer, clearingColor.r, clearingColor.g, clearingColor.b, clearingColor.a);
	SDL_SetRenderDrawColor(xzRenderer, clearingColor.r, clearingColor.g, clearingColor.b, clearingColor.a);
	SDL_SetRenderDrawColor(yzRenderer, clearingColor.r, clearingColor.g, clearingColor.b, clearingColor.a);
	SDL_SetRenderDrawColor(dataRenderer, clearingColor.r, clearingColor.g, clearingColor.b, clearingColor.a);
	SDL_RenderClear(xyRenderer);
	SDL_RenderClear(xzRenderer);
	SDL_RenderClear(yzRenderer);
	SDL_RenderClear(dataRenderer);
}

void PESO_Graphics::PESO_ShowScreen() {
	SDL_RenderPresent(xyRenderer);
	SDL_RenderPresent(xzRenderer);
	SDL_RenderPresent(yzRenderer);
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
