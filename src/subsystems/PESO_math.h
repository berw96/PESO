/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _MATH_
#define _MATH_

#include <cstdlib>
#include <climits>
#include <SDL_rect.h>

#pragma region CONSTANTS
static const double _PI_OVER_180_ = (double)(3.14159265358979323846 / 180);
static const double _180_OVER_PI_ = (double)(180 / 3.14159265358979323846);
static const double _PI_ = 3.14159265358979323846;
#pragma endregion

#pragma region GEOMETRY
struct Point2d {
	double horizontal;
	double vertical;

	Point2d() : Point2d(0, 0) {}
	Point2d(double horizontal, double vertical) : horizontal(horizontal), vertical(vertical) {}
};

struct Line2i {
	Point2d startPoint;
	Point2d endPoint;

	Line2i() : Line2i(Point2d(), Point2d()) {}
	Line2i(const Point2d& startPoint, const Point2d& endPoint) : startPoint(startPoint), endPoint(endPoint) {}
};

struct Rectangle2d {
	double x;
	double y;
	double width;
	double height;

	Rectangle2d() : Rectangle2d(0.0, 0.0, 1.0, 1.0) {}
	Rectangle2d(double x, double y, double width, double height) : x(x), y(y), width(width), height(height) {}

	SDL_Rect getSDLRectFrom() const {
		SDL_Rect rectangle = {x, y, width, height};
		return rectangle;
	}

	inline bool contains(const Point2d& point) {
		return
			point.horizontal >= x &&
			point.horizontal <= x + width &&
			point.vertical >= y &&
			point.vertical <= y + height;
	}


	inline bool intersects(const Rectangle2d& other) {
		SDL_Rect rectangleA = getSDLRectFrom();
		SDL_Rect rectangleB = other.getSDLRectFrom();

		return SDL_HasIntersection(&rectangleA, &rectangleB) == SDL_TRUE;
	}

	inline bool intersects(const Line2i& line) {
		SDL_Rect rectangle = {x, y, width, height};
		int x1 = line.startPoint.horizontal;
		int y1 = line.startPoint.vertical;
		int x2 = line.endPoint.horizontal;
		int y2 = line.endPoint.vertical;

		return SDL_IntersectRectAndLine(&rectangle, &x1, &y1, &x2, &y2) == SDL_TRUE;
	}
};
#pragma endregion

#endif
