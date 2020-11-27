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

#pragma region TRANSFORM
struct Vector3d {
	double x;
	double y;
	double z;

	Vector3d() : Vector3d(0, 0, 0) {}
	Vector3d(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct Point2i {
	double x;
	double y;

	Point2i() : Point2i(0, 0) {}
	Point2i(double x, double y) : x(x), y(y) {}
};
#pragma endregion

#pragma region GEOMETRY
struct Line2i {
	Point2i startPoint;
	Point2i endPoint;

	Line2i() : Line2i(Point2i(), Point2i()) {}
	Line2i(const Point2i& startPoint, const Point2i& endPoint) : startPoint(startPoint), endPoint(endPoint) {}
};

struct Rectangle2d {
	double x;
	double y;
	double width;
	double height;

	Rectangle2d(double x, double y, double width, double height) : x(x), y(y), width(width), height(height) {}

	SDL_Rect getSDLRect() const {
		SDL_Rect rectangle = {x, y, width, height};
		return rectangle;
	}

	inline bool contains(const Point2i& point) {
		return
			point.x >= x &&
			point.x <= x + width &&
			point.y >= y &&
			point.y <= y + height;
	}

	inline bool intersects(const Rectangle2d& other) {
		SDL_Rect rectangleA = getSDLRect();
		SDL_Rect rectangleB = other.getSDLRect();

		return SDL_HasIntersection(&rectangleA, &rectangleB) == SDL_TRUE;
	}

	inline bool intersects(const Line2i& line) {
		SDL_Rect rectangle = {x, y, width, height};
		int x1 = line.startPoint.x;
		int y1 = line.startPoint.y;
		int x2 = line.endPoint.x;
		int y2 = line.endPoint.y;

		return SDL_IntersectRectAndLine(&rectangle, &x1, &y1, &x2, &y2) == SDL_TRUE;
	}
};
#pragma endregion




#endif
