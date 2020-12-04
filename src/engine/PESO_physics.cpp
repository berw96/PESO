/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>
*/

#include "PESO_physics.h"
#include <iostream>

#pragma region PESO_OBJECT
PESO_Object::PESO_Object() { setMass(_DEFAULT_INIT_MASS_); setTag(_DEFAULT_TAG_); };
PESO_Object::PESO_Object(const Vector3d& centre) : centre(centre) { setMass(_DEFAULT_INIT_MASS_); setTag(_DEFAULT_TAG_); };
PESO_Object::PESO_Object(const Vector3d& centre, Vector3d pivotPoint) : centre(centre), pivotPoint(pivotPoint) { setMass(_DEFAULT_INIT_MASS_); };
PESO_Object::PESO_Object(const Vector3d& centre, Vector3d pivotPoint, float mass) : centre(centre), pivotPoint(pivotPoint) { setMass(mass); setTag(_DEFAULT_TAG_); };
PESO_Object::PESO_Object(const Vector3d& centre, Vector3d pivotPoint, float mass, PESO_Transform transform) : centre(centre), pivotPoint(pivotPoint), transform(transform) { setMass(mass); setTag(_DEFAULT_TAG_); };
PESO_Object::PESO_Object(const Vector3d& centre, Vector3d pivotPoint, float mass, PESO_Transform transform, float radius) : centre(centre), pivotPoint(pivotPoint), transform(transform), radius(radius) { setMass(mass); setTag(_DEFAULT_TAG_); };
PESO_Object::PESO_Object(const Vector3d& centre, Vector3d pivotPoint, float mass, PESO_Transform transform, float radius, std::string tag) : centre(centre), pivotPoint(pivotPoint), transform(transform), radius(radius) { setMass(mass); setTag(tag); };
PESO_Object::PESO_Object(const Vector3d& centre, Vector3d pivotPoint, float mass, PESO_Transform transform, float radius, std::string tag, Vector3d thrust) : centre(centre), pivotPoint(pivotPoint), transform(transform), radius(radius), thrust(thrust) { setMass(mass); setTag(tag); };
#pragma endregion

#pragma region PESO_PHYSICS
PESO_Physics::PESO_Physics() {};

void PESO_Physics::PESO_registerObject(std::shared_ptr<PESO_Object> obj) {
	objects.push_back(obj);
};

void PESO_Physics::PESO_ApplyLinearMechanics() {
	for (auto obj : objects) {
		PESO_CalculateNetLinForce(*obj);

		//calculate linear acceleration
		obj->linAcceleration.x = obj->netLinForce.x / obj->mass;
		obj->linAcceleration.y = obj->netLinForce.y / obj->mass;
		obj->linAcceleration.z = obj->netLinForce.z / obj->mass;

		//accelerate
		obj->linSpeed.x += obj->linAcceleration.x;
		obj->linSpeed.y += obj->linAcceleration.y;
		obj->linSpeed.z += obj->linAcceleration.z;

		//set magnitude of linear velocity
		obj->linVelocity.x = obj->linSpeed.x;
		obj->linVelocity.y = obj->linSpeed.y;
		obj->linVelocity.z = obj->linSpeed.z;

		//move
		obj->transform.position.x += obj->linVelocity.x;
		obj->transform.position.y += obj->linVelocity.y;
		obj->transform.position.z += obj->linVelocity.z;

		//geometric attributes follow position
		obj->centre.x		+= obj->transform.position.x;
		obj->centre.y		+= obj->transform.position.y;
		obj->centre.z		+= obj->transform.position.z;
		obj->pivotPoint.x	+= obj->transform.position.x;
		obj->pivotPoint.y	+= obj->transform.position.y;
		obj->pivotPoint.z	+= obj->transform.position.z;
		obj->centreOfMass.x += obj->transform.position.x;
		obj->centreOfMass.y += obj->transform.position.y;
		obj->centreOfMass.z += obj->transform.position.z;
	}
};

void PESO_Physics::PESO_ApplyRotationMechanics() {

};

Vector3d PESO_Physics::PESO_CalculateGravForce(PESO_Object& target, PESO_Object& satellite) {
	double gravForce = ((target.mass * satellite.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(PESO_CalculateRange(target, satellite), 2));
	Vector3d satellite_to_target (
		(target.transform.position.x - satellite.transform.position.x) * gravForce,
		(target.transform.position.y - satellite.transform.position.y) * gravForce,
		(target.transform.position.z - satellite.transform.position.z) * gravForce
	);
	return satellite_to_target;
};

Vector3d PESO_Physics::PESO_CalculateNetGravForce(PESO_Object& satellite) {
	double gravForce = 0.0;
	Vector3d satellite_to_target = Vector3d();
	for (auto obj : objects) {
		if (obj->getTag() != satellite.getTag()) {
			gravForce += ((obj->mass * satellite.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(PESO_CalculateRange(*obj, satellite), 2));
			satellite_to_target.x += (obj->transform.position.x - satellite.transform.position.x) * gravForce;
			satellite_to_target.y += (obj->transform.position.y - satellite.transform.position.y) * gravForce;
			satellite_to_target.z += (obj->transform.position.z - satellite.transform.position.z) * gravForce;
		}
	}
	return satellite_to_target;
};

void PESO_Physics::PESO_CalculateNetLinForce(PESO_Object& object) {
	object.netLinForce.x = PESO_CalculateNetGravForce(object).x + object.thrust.x;
	object.netLinForce.y = PESO_CalculateNetGravForce(object).x + object.thrust.y;
	object.netLinForce.z = PESO_CalculateNetGravForce(object).z + object.thrust.z;
};

void PESO_Physics::PESO_CalculateNetAngForce(PESO_Object& object) {

};

double PESO_Physics::PESO_CalculateRange(PESO_Object& target, PESO_Object& satellite) {
	Vector3d range(
		target.transform.position.x - satellite.transform.position.x,
		target.transform.position.y - satellite.transform.position.y,
		target.transform.position.z - satellite.transform.position.z
	);
	double res_range = PESO_CalculateResultant(range);
	if (res_range == 0.0) {
		res_range = DBL_MIN;
	}
	return res_range;
};

double PESO_Physics::PESO_CalculateResultant(Vector3d v) {
	double res = (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
	return res;
};

void PESO_Physics::PESO_CalculateCentreOfMass(PESO_Object& object) {
	for (auto comp : object.components) {

	}
};

void PESO_Physics::PESO_CalculateLinMomentum(PESO_Object& object) {
	object.linMomentum.x = object.linVelocity.x * object.mass;
	object.linMomentum.y = object.linVelocity.y * object.mass;
	object.linMomentum.z = object.linVelocity.z * object.mass;
};

void PESO_Physics::PESO_CalculateOrbitPeriod(PESO_Object& satellite, PESO_Object& target) {
	satellite.period = sqrt(2 * _PI_ * pow(PESO_CalculateRange(satellite, target), 3) / _UNIVERSAL_CONST_GRAVITATION_ * satellite.mass);
};

void PESO_Physics::PESO_CalculateReqLinVelocity(PESO_Object& satellite, PESO_Object& target) {
	double reqVelocity = sqrt(_UNIVERSAL_CONST_GRAVITATION_ * target.mass / PESO_CalculateRange(satellite, target));
	satellite.reqLinVelocity = Vector3d(
		(target.transform.position.x - satellite.transform.position.x) * reqVelocity,
		(target.transform.position.y - satellite.transform.position.y) * reqVelocity,
		(target.transform.position.z - satellite.transform.position.z) * reqVelocity
	);
};
#pragma endregion
