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
#define _EARTH_MASS_					(5972000000000000000000000.f)
#define _PIXEL_TO_METER_				(3846.153846f)
#define _METER_TO_PIXEL_				(float)(1.f / 3846.153846f)

#include <iostream>
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

//prototypes
class PESO_Object;
struct PESO_Data;

class PESO_Physics {
	friend class PESO;
	friend class PESO_Object;

#pragma region IMPLEMENTATION
	std::vector<std::shared_ptr<PESO_Object>> objects;
	std::vector<PESO_Data> sessionData;

public:
	PESO_Physics();
	void PESO_RegisterObject(std::shared_ptr<PESO_Object> object);
	void PESO_LogData(PESO_Data objectData);
	std::vector<PESO_Data> PESO_GetSessionData() const { return this->sessionData; };
	void PESO_ApplyLinearMechanics();
	void PESO_ApplyRotationMechanics();
#pragma endregion

#pragma region FORMULAE
	Vector3d PESO_CalculateGravForce(PESO_Object& target, PESO_Object& satellite);
	Vector3d PESO_CalculateNetGravForce(PESO_Object& satellite);

	Vector3d PESO_CalculateNetAero(PESO_Object& satellite);

	double PESO_CalculateResultant(Vector3d v);
	double PESO_CalculateRange(PESO_Object& a, PESO_Object& b);

	double PESO_CalculateEccentricityXY(PESO_Object& target, PESO_Object& satellite);
	double PESO_CalculateEccentricityXZ(PESO_Object& target, PESO_Object& satellite);
	double PESO_CalculateEccentricityYZ(PESO_Object& target, PESO_Object& satellite);

	double PESO_CalculuateOrbitAreaXY(PESO_Object& target, PESO_Object& satellite);
	double PESO_CalculuateOrbitAreaXZ(PESO_Object& target, PESO_Object& satellite);
	double PESO_CalculuateOrbitAreaYZ(PESO_Object& target, PESO_Object& satellite);
	
	void PESO_CalculateOrbitPeriod(PESO_Object& target, PESO_Object& satellite);
	void PESO_CalculateReqLinVelocity(PESO_Object& target, PESO_Object& satllite);
	void PESO_CalculateLinMomentum(PESO_Object& object);
	void PESO_CalculateNetLinForce(PESO_Object& object);
	void PESO_CalculateNetAngForce(PESO_Object& object);
	void PESO_CalculateCentreOfMass(PESO_Object& object);
#pragma endregion
};

struct PESO_Data {
	std::string tag;
	time_t timestamp;
#pragma region FIELDS
	Vector3d centre;
	Vector3d pivotPoint;

	PESO_Transform transform;

	Vector3d netLinForce;			//Net force enacted on the object
	Vector3d gravForce;				//Gravitational force experienced by the object
	Vector3d thrust;				//Aerodynamic thrust produced by the object
	Vector3d drag;					//Aerodynamic drag experienced by the object
	Vector3d lift;					//Aerodynamic lift experienced by the object
	Vector3d downforce;				//Aerodynamic downforce experienced by the object
	Vector3d linSpeed;				//Linear speed of object (magnitude of linVelocity)
	Vector3d linAcceleration;		//Linear acceleration of object
	Vector3d linVelocity;			//Linear velocity of object
	Vector3d linMomentum;			//Linear momentum of object
	Vector3d reqVelocity;			//Linear velocity required to maintain orbit radius at point

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
	double fuel;					//Object's fuel count
	bool burning;					//Object thrust toggle
#pragma endregion

#pragma region CONSTRUCTORS
	PESO_Data();
	PESO_Data(const Vector3d& centre);
	PESO_Data(const Vector3d& centre, Vector3d pivotPoint);
	PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass);
	PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform);
	PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius);
	PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius, std::string tag);
	PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius, std::string tag, Vector3d thrust);
#pragma endregion

#pragma region GETTERS
	time_t getTimestamp()	const { return timestamp; };
#pragma endregion
};

class PESO_Object {
	friend class PESO_Physics;
protected:
	//object's physical data is stored in its PESO_Data instance
	PESO_Data objectData;

	//an object may have others as components
	std::vector<PESO_Object> children;

public:
#pragma region CONSTRUCTORS
	PESO_Object();
	PESO_Object(PESO_Data data);
#pragma endregion

#pragma region GETTERS
	PESO_Data getObjectData()		const { return objectData; };
	std::string getTag()			const { return objectData.tag; };

	Vector3d getCentre()			const { return objectData.centre; };
	Vector3d getCentreOfMass()		const { return objectData.centreOfMass; };
	Vector3d getPivotPoint()		const { return objectData.pivotPoint; };

	PESO_Transform getTransform()	const { return objectData.transform; };

	Vector3d getPosition()			const { return objectData.transform.position; };
	Vector3d getRotation()			const { return objectData.transform.rotation; };
	Vector3d getNetLinForce()		const { return objectData.netLinForce; };
	Vector3d getNetAngForce()		const { return objectData.netAngForce; };
	Vector3d getGravForce()			const { return objectData.gravForce; };
	Vector3d getThrust()			const { return objectData.thrust; };
	Vector3d getLinAcceleration()	const { return objectData.linAcceleration; };
	Vector3d getAngAcceleration()	const { return objectData.angAcceleration; };
	Vector3d getLinVelocity()		const { return objectData.linVelocity; };
	Vector3d getAngVelocity()		const { return objectData.angVelocity; };
	Vector3d getLinSpeed()			const { return objectData.linSpeed; };
	Vector3d getAngSpeed()			const { return objectData.angSpeed; };
	Vector3d getLinMomentum()		const { return objectData.linMomentum; };
	Vector3d getAngMomentum()		const { return objectData.angMomentum; };

	double getMass()				const { return objectData.mass; };
	double getPeriod()				const { return objectData.period; };
	double getRadius()				const { return objectData.radius; };
#pragma endregion

#pragma region SETTERS
	void setPosition(Vector3d pos) { this->objectData.transform.position = pos; }
	void setRotation(Vector3d rot) { this->objectData.transform.rotation = rot; }
	void setMass(double mass) {
		if (mass <= 0 || mass == NULL) {
			mass = DBL_MIN;
		}
		this->objectData.mass = mass;
	}
	void setTag(std::string tag)	{ this->objectData.tag = tag; }
	void setThrust(Vector3d thrust) { this->objectData.thrust = thrust; };
#pragma endregion
};

#endif
