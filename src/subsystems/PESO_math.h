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
#pragma endregion

#endif
