/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _PHYSICS_
#define _PHYSICS_
#define _UNIVERSAL_CONST_GRAVITATION_ (0.00000000006674000000f)
#define _DEFAULT_INIT_MASS_ (1.f)
#define _DEFAULT_TAG_ ("")
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
	std::vector<std::shared_ptr<PESO_Object>> objects;

public:
	PESO_Physics();
	void PESO_RegisterObject(std::shared_ptr<PESO_Object> object);
	void PESO_ApplyLinearMechanics();
	void PESO_ApplyRotationMechanics();
#pragma endregion

#pragma region FORMULA
	Vector3d PESO_CalculateGravForce(PESO_Object& target, PESO_Object& satellite);
	Vector3d PESO_CalculateNetGravForce(PESO_Object& satellite);

	double PESO_CalculateResultant(Vector3d v);
	double PESO_CalculateRange(PESO_Object& a, PESO_Object& b);
	double PESO_CalculateEccentricity(PESO_Object& target, PESO_Object& satellite);
	double PESO_CalculuateOrbitArea(PESO_Object& target, PESO_Object& satellite);
	
	void PESO_CalculateOrbitPeriod(PESO_Object& target, PESO_Object& satellite);
	void PESO_CalculateReqLinVelocity(PESO_Object& target, PESO_Object& satllite);
	void PESO_CalculateLinMomentum(PESO_Object& object);
	void PESO_CalculateNetLinForce(PESO_Object& object);
	void PESO_CalculateNetAngForce(PESO_Object& object);
	void PESO_CalculateCentreOfMass(PESO_Object& object);
#pragma endregion
};

class PESO_Object {
	friend class PESO_Physics;
protected:
	std::string tag;
	//each object may possess a number of components.
	std::vector<std::shared_ptr<PESO_Object>> components;
#pragma region FIELDS
	Vector3d centre;
	Vector3d pivotPoint;

	PESO_Transform transform;

	Vector3d netLinForce;
	Vector3d gravForce;
	Vector3d thrust;
	Vector3d linSpeed;
	Vector3d linAcceleration;
	Vector3d linVelocity;
	Vector3d linMomentum;
	Vector3d reqLinVelocity;

	Vector3d torque;
	Vector3d netAngForce;
	Vector3d angSpeed;
	Vector3d angAcceleration;
	Vector3d angVelocity;
	Vector3d angMomentum;

	Vector3d centreOfMass;

	double mass;
	double inertia;
	double period;
	double radius;
#pragma endregion

#pragma region CONSTRUCTORS
public:
	PESO_Object();
	PESO_Object(const Vector3d& centre);
	PESO_Object(const Vector3d& centre, Vector3d pivotPoint);
	PESO_Object(const Vector3d& centre, Vector3d pivotPoint, double mass);
	PESO_Object(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform);
	PESO_Object(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius);
	PESO_Object(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius, std::string tag);
	PESO_Object(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius, std::string tag, Vector3d thrust);
#pragma endregion

#pragma region GETTERS
	std::string getTag()			const { return tag; };

	Vector3d getCentre()			const { return centre; };
	Vector3d getCentreOfMass()		const { return centreOfMass; };
	Vector3d getPivotPoint()		const { return pivotPoint; };

	PESO_Transform getTransform()	const { return transform; };

	Vector3d getPosition()			const { return transform.position; };
	Vector3d getRotation()			const { return transform.rotation; };
	Vector3d getNetLinForce()		const { return netLinForce; };
	Vector3d getNetAngForce()		const { return netAngForce; };
	Vector3d getGravForce()			const { return gravForce; };
	Vector3d getThrust()			const { return thrust; };
	Vector3d getlinAcceleration()	const { return linAcceleration; };
	Vector3d getAngAcceleration()	const { return angAcceleration; };
	Vector3d getlinVelocity()		const { return linVelocity; };
	Vector3d getAngVelocity()		const { return angVelocity; };
	Vector3d getLinSpeed()			const { return linSpeed; };
	Vector3d getAngSpeed()			const { return angSpeed; };
	Vector3d getlinMomentum()		const { return linMomentum; };
	Vector3d getAngMomentum()		const { return angMomentum; };

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
