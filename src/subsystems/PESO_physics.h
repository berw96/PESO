/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#ifndef _PHYSICS_
#define _PHYSICS_
#define _UNIVERSAL_CONST_GRAVITATION_	(0.00000000006674000000f)
#define _DEFAULT_INIT_MASS_				(1.f)
#define _DEFAULT_TAG_					("")
#define _DEFAULT_RADIUS_				(10.f)

#include <vector>
#include <string>
#include <memory>
#include "PESO_math.h"
#include "PESO_timer.h"

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
#pragma region FIELDS
	Vector3d centre;
	Vector3d pivotPoint;

	PESO_Transform transform;

	Vector3d netLinForce;			//Net force enacted on the object
	Vector3d gravForce;				//Gravitational force experienced by the object
	Vector3d thrust;				//Thrust produced by the object
	Vector3d drag;					//Aerodynamic drag experienced by the object
	Vector3d lift;					//Aerodynamic lift experienced by the object
	Vector3d downforce;				//Aerodynamic downforce experienced by the object
	Vector3d linSpeed;				//Linear speed of object (magnitude of linVelocity)
	Vector3d linAcceleration;		//Linear acceleration of object
	Vector3d linVelocity;			//Linear velocity of object
	Vector3d linMomentum;			//Linear momentum of object
	Vector3d reqLinVelocity;		//Linear velocity required to maintain orbit radius at point

	Vector3d torque;				//Turning force of object
	Vector3d netAngForce;			//Net angular force experienced by object
	Vector3d angSpeed;				//Angular speed of object (magnitude of angVelocity)
	Vector3d angAcceleration;		//Angular acceleration of object
	Vector3d angVelocity;			//Angular velcoty of object
	Vector3d angMomentum;			//Angular momentum of object

	Vector3d centreOfMass;			//Equivalent to centre of balance

	double mass;					//Mass of object
	double inertia;					//Object's resistance to changes in rotational motion
	double period;					//Object's orbital period
	double radius;					//Object's geometric radius (if applicable)
	bool burning;					//Object thrust toggle
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
	Vector3d getLinAcceleration()	const { return linAcceleration; };
	Vector3d getAngAcceleration()	const { return angAcceleration; };
	Vector3d getLinVelocity()		const { return linVelocity; };
	Vector3d getAngVelocity()		const { return angVelocity; };
	Vector3d getLinSpeed()			const { return linSpeed; };
	Vector3d getAngSpeed()			const { return angSpeed; };
	Vector3d getLinMomentum()		const { return linMomentum; };
	Vector3d getAngMomentum()		const { return angMomentum; };

	double getMass()				const { return mass; };
	double getPeriod()				const { return period; };
	double getRadius()				const { return radius; };
#pragma endregion

#pragma region SETTERS
	void setPosition(Vector3d pos) { this->transform.position = pos; }
	void setRotation(Vector3d rot) { this->transform.rotation = rot; }
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
