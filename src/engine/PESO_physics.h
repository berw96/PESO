/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _PHYSICS_
#define _PHYSICS_
#define _UNIVERSAL_CONST_GRAVITATION_ (0.00000000006674000000f)
#define _DEFAULT_INIT_MASS_ (1.f)
#define _DEFAULT_RADIUS_ (10.f)

#include <vector>
#include <string>
#include <memory>
#include "PESO_math.h"

struct Vector3d {
	double x;
	double y;
	double z;

	Vector3d() : Vector3d(0, 0, 0) {}
	Vector3d(double x, double y, double z) : x(x), y(y), z(z) {}
};

struct PESO_Transform {
	Vector3d position;
	Vector3d rotation;

	PESO_Transform() : PESO_Transform(Vector3d(), Vector3d()) {}
	PESO_Transform(Vector3d position, Vector3d rotation) : position(position), rotation(rotation) {}
};

class PESO_Object;

class PESO_Physics {
	friend class PESO;
	friend class PESO_Object;

#pragma region IMPLEMENTATION
	PESO_Physics();
	std::vector<std::shared_ptr<PESO_Object>> objects;

public:
	void PESO_registerObject(std::shared_ptr<PESO_Object> object);
	void PESO_ApplyMechanics();
#pragma endregion

#pragma region FORMULA
	Vector3d PESO_CalculateGravForce(PESO_Object& target, PESO_Object& satellite);
	Vector3d PESO_CalculateNetGravForce(PESO_Object& satellite);

	double PESO_CalculateResultant(Vector3d v);
	double PESO_CalculateRange(PESO_Object& a, PESO_Object& b);

	void PESO_CalculateOrbitalPeriod(PESO_Object& target, PESO_Object& satellite);
	void PESO_CalculateReqVelocity(PESO_Object& target, PESO_Object& satllite);
	void PESO_CalculateMomentum(PESO_Object& object);
	void PESO_CalculateNetForce(PESO_Object& object);
#pragma endregion
};

class PESO_Object {
	friend class PESO_Physics;
protected:
	std::string tag;
#pragma region FIELDS
	Point2i centre;
	Point2i pivotPoint;

	PESO_Transform transform;

	Vector3d netForce;
	Vector3d gravForce;
	Vector3d thrust;
	Vector3d acceleration;
	Vector3d velocity;
	Vector3d reqVelocity;
	Vector3d speed;
	Vector3d momentum;

	double mass;
	double period;
	double radius;
#pragma endregion

#pragma region CONSTRUCTORS
	PESO_Object(const Point2i& center);
	PESO_Object(const Point2i& center, Point2i pivotPoint);
	PESO_Object(const Point2i& center, Point2i pivotPoint, float mass);
	PESO_Object(const Point2i& center, Point2i pivotPoint, float mass, PESO_Transform transform);
	PESO_Object(const Point2i& center, Point2i pivotPoint, float mass, PESO_Transform transform, float radius);
	PESO_Object(const Point2i& center, Point2i pivotPoint, float mass, PESO_Transform transform, float radius, std::string tag);
	PESO_Object(const Point2i& center, Point2i pivotPoint, float mass, PESO_Transform transform, float radius, std::string tag, Vector3d thrust);
#pragma endregion

#pragma region GETTERS
	std::string getTag()			const { return tag; };

	Point2i getCentre()				const { return centre; };
	Point2i getPivotPoint()			const { return pivotPoint; };

	PESO_Transform getTransform()	const { return transform; };

	Vector3d getPosition()			const { return transform.position; };
	Vector3d getRotation()			const { return transform.rotation; };
	Vector3d getNetForce()			const { return netForce; };
	Vector3d getGravForce()			const { return gravForce; };
	Vector3d getThrust()			const { return thrust; };
	Vector3d getAcceleration()		const { return acceleration; };
	Vector3d getVelocity()			const { return velocity; };
	Vector3d getSpeed()				const { return speed; };
	Vector3d getMomentum()			const { return momentum; };

	double getMass()				const { return mass; };
	double getPeriod()				const { return period; };
	double getRadius()				const { return radius; };
#pragma endregion

#pragma region SETTERS
	void setMass(double mass) {
		if (mass <= 0) {
			mass = DBL_MIN;
		}
		this->mass = mass;
	}
	void setTag(std::string tag)	{ this->tag = tag; }
	void setThrust(Vector3d thrust) { this->thrust = thrust; };
#pragma endregion
};

#endif
