/*
	Physics Engine for Simulating Orbits.
	Copyright (C) 2020-2021 Elliot R. Walker <berw96@gmail.com>

	Made with C++ and Simple DirectMedia Layer (SDL) Version 2.0, under the
	GNU General Public Licence (GPL) and the ZLib Licence.
*/

#include "PESO_physics.h"

#pragma region PESO_DATA
PESO_Data::PESO_Data() : mass(_DEFAULT_INIT_MASS_), tag(_DEFAULT_TAG_), radius(_DEFAULT_RADIUS_), pivotPoint(_DEFAULT_PIVOT_POINT_), thrust(_DEFAULT_THRUST_), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre) : centre(centre), mass(_DEFAULT_INIT_MASS_), tag(_DEFAULT_TAG_), radius(_DEFAULT_RADIUS_), pivotPoint(_DEFAULT_PIVOT_POINT_), thrust(_DEFAULT_THRUST_), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre, Vector3d pivotPoint) : centre(centre), pivotPoint(pivotPoint), mass(_DEFAULT_INIT_MASS_), tag(_DEFAULT_TAG_), radius(_DEFAULT_RADIUS_), thrust(_DEFAULT_THRUST_), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass) : centre(centre), pivotPoint(pivotPoint), mass(mass), tag(_DEFAULT_TAG_), radius(_DEFAULT_RADIUS_), thrust(_DEFAULT_THRUST_), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform) : centre(centre), pivotPoint(pivotPoint), transform(transform), mass(mass), tag(_DEFAULT_TAG_), radius(_DEFAULT_RADIUS_), thrust(_DEFAULT_THRUST_), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius) : centre(centre), pivotPoint(pivotPoint), transform(transform), radius(radius), mass(mass), tag(_DEFAULT_TAG_), thrust(_DEFAULT_THRUST_), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius, std::string tag) : centre(centre), pivotPoint(pivotPoint), transform(transform), radius(radius), mass(mass), tag(tag), thrust(_DEFAULT_THRUST_), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius, std::string tag, Vector3d thrust) : centre(centre), pivotPoint(pivotPoint), transform(transform), radius(radius), thrust(thrust), mass(mass), tag(tag), torque(_DEFUALT_TORQUE_) {};
PESO_Data::PESO_Data(const Vector3d& centre, Vector3d pivotPoint, double mass, PESO_Transform transform, double radius, std::string tag, Vector3d thrust, Vector3d torque) : centre(centre), pivotPoint(pivotPoint), transform(transform), radius(radius), thrust(thrust), mass(mass), tag(tag), torque(torque) {};
#pragma endregion

#pragma region PESO_OBJECT
PESO_Object::PESO_Object() {};
PESO_Object::PESO_Object(PESO_Data data) : objectData(data) { setMass(data.mass); };
#pragma endregion

#pragma region PESO_PHYSICS
PESO_Physics::PESO_Physics() {};

void PESO_Physics::PESO_RegisterObject(std::shared_ptr<PESO_Object> obj) {
	if(obj != nullptr)
		objects.push_back(obj);
};

void PESO_Physics::PESO_LogData(PESO_Data objectData) {
	objectData.timestamp = time(NULL);
	sessionData.push_back(objectData);
	std::cout << "Data Logged: " << ctime(&objectData.timestamp);
};

void PESO_Physics::PESO_ApplyLinearMechanics() {
	for (auto obj : objects) {
		PESO_CalculateNetLinForce(*obj);
		PESO_CalculateLinMomentum(*obj);

		//calculate linear acceleration
		obj->objectData.linAcceleration.x = obj->objectData.netLinForce.x / obj->objectData.mass;
		obj->objectData.linAcceleration.y = obj->objectData.netLinForce.y / obj->objectData.mass;
		obj->objectData.linAcceleration.z = obj->objectData.netLinForce.z / obj->objectData.mass;

		//accelerate
		obj->objectData.linSpeed.x += obj->objectData.linAcceleration.x;
		obj->objectData.linSpeed.y += obj->objectData.linAcceleration.y;
		obj->objectData.linSpeed.z += obj->objectData.linAcceleration.z;

		//set magnitude of linear velocity
		obj->objectData.linVelocity.x = obj->objectData.linSpeed.x;
		obj->objectData.linVelocity.y = obj->objectData.linSpeed.y;
		obj->objectData.linVelocity.z = obj->objectData.linSpeed.z;

		//move
		obj->objectData.transform.position.x += obj->objectData.linVelocity.x;
		obj->objectData.transform.position.y += obj->objectData.linVelocity.y;
		obj->objectData.transform.position.z += obj->objectData.linVelocity.z;

		//geometric attributes follow position
		obj->objectData.centre.x		+= obj->objectData.transform.position.x;
		obj->objectData.centre.y		+= obj->objectData.transform.position.y;
		obj->objectData.centre.z		+= obj->objectData.transform.position.z;

		obj->objectData.pivotPoint.x	+= obj->objectData.transform.position.x;
		obj->objectData.pivotPoint.y	+= obj->objectData.transform.position.y;
		obj->objectData.pivotPoint.z	+= obj->objectData.transform.position.z;

		obj->objectData.centreOfMass.x	+= obj->objectData.transform.position.x;
		obj->objectData.centreOfMass.y	+= obj->objectData.transform.position.y;
		obj->objectData.centreOfMass.z	+= obj->objectData.transform.position.z;
	}
};

void PESO_Physics::PESO_ApplyRotationMechanics() {
	for (auto obj : objects) {
		PESO_CalculateInertia(*obj);
		PESO_CalculateNetAngForce(*obj);

		// Reset rotation if a full cycle has been achieved
		if (obj->objectData.transform.rotation.x >= 360.0 ||
			obj->objectData.transform.rotation.x <= -360.0)
			obj->objectData.transform.rotation.x = 0.0;
		
		if (obj->objectData.transform.rotation.y >= 360.0 ||
			obj->objectData.transform.rotation.y <= -360.0) 
			obj->objectData.transform.rotation.y = 0.0;
		
		if (obj->objectData.transform.rotation.z >= 360.0 ||
			obj->objectData.transform.rotation.z <= -360.0)
			obj->objectData.transform.rotation.z = 0.0;

		obj->objectData.angAcceleration.x = obj->objectData.netAngForce.x / obj->objectData.inertia;
		obj->objectData.angAcceleration.y = obj->objectData.netAngForce.y / obj->objectData.inertia;
		obj->objectData.angAcceleration.z = obj->objectData.netAngForce.z / obj->objectData.inertia;

		obj->objectData.angSpeed.x += obj->objectData.angAcceleration.x;
		obj->objectData.angSpeed.y += obj->objectData.angAcceleration.y;
		obj->objectData.angSpeed.z += obj->objectData.angAcceleration.z;

		obj->objectData.angVelocity.x = obj->objectData.angSpeed.x;
		obj->objectData.angVelocity.y = obj->objectData.angSpeed.y;
		obj->objectData.angVelocity.z = obj->objectData.angSpeed.z;

		obj->objectData.transform.rotation.x += obj->objectData.angVelocity.x;
		obj->objectData.transform.rotation.y += obj->objectData.angVelocity.y;
		obj->objectData.transform.rotation.z += obj->objectData.angVelocity.z;
	}
};

Vector3d PESO_Physics::PESO_CalculateGravForce(PESO_Object& target, PESO_Object& satellite) {
	double gravForce = ((target.objectData.mass * satellite.objectData.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(PESO_CalculateRange(target, satellite), 2));
	Vector3d satellite_to_target (
		(target.objectData.transform.position.x - satellite.objectData.transform.position.x) * gravForce,
		(target.objectData.transform.position.y - satellite.objectData.transform.position.y) * gravForce,
		(target.objectData.transform.position.z - satellite.objectData.transform.position.z) * gravForce
	);
	return satellite_to_target;
};

Vector3d PESO_Physics::PESO_CalculateNetGravForce(PESO_Object& satellite) {
	double gravForce = 0.0;
	Vector3d satellite_to_target = Vector3d();
	for (auto obj : objects) {
		if (obj->getTag() != satellite.getTag()) {
			gravForce += ((obj->objectData.mass * satellite.objectData.mass * _UNIVERSAL_CONST_GRAVITATION_) / pow(PESO_CalculateRange(*obj, satellite), 2));
			satellite_to_target.x += (obj->objectData.transform.position.x - satellite.objectData.transform.position.x) * gravForce;
			satellite_to_target.y += (obj->objectData.transform.position.y - satellite.objectData.transform.position.y) * gravForce;
			satellite_to_target.z += (obj->objectData.transform.position.z - satellite.objectData.transform.position.z) * gravForce;
		}
	}
	return satellite_to_target;
};

void PESO_Physics::PESO_CalculateNetLinForce(PESO_Object& object) {
	object.objectData.netLinForce.x = PESO_CalculateNetGravForce(object).x + (object.objectData.thrust.x * cos(object.objectData.transform.rotation.y) * cos(object.objectData.transform.rotation.z));
	object.objectData.netLinForce.y = PESO_CalculateNetGravForce(object).y + (object.objectData.thrust.y * cos(object.objectData.transform.rotation.x) * cos(object.objectData.transform.position.z));
	object.objectData.netLinForce.z = PESO_CalculateNetGravForce(object).z + (object.objectData.thrust.z * cos(object.objectData.transform.rotation.x) * cos(object.objectData.transform.rotation.y));
};

void PESO_Physics::PESO_CalculateNetAngForce(PESO_Object& object) {
	object.objectData.netAngForce.x = object.objectData.torque.x;
	object.objectData.netAngForce.y = object.objectData.torque.y;
	object.objectData.netAngForce.z = object.objectData.torque.z;
};

double PESO_Physics::PESO_CalculateRange(PESO_Object& target, PESO_Object& satellite) {
	Vector3d range(
		target.objectData.transform.position.x - satellite.objectData.transform.position.x,
		target.objectData.transform.position.y - satellite.objectData.transform.position.y,
		target.objectData.transform.position.z - satellite.objectData.transform.position.z
	);
	double res_range = PESO_CalculateResultant(range);
	if (res_range == 0.0) {
		res_range = DBL_MIN;
	}
	return abs(res_range);
};

double PESO_Physics::PESO_CalculateResultant(Vector3d v) {
	double res = (sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)));
	return res;
};

void PESO_Physics::PESO_CalculateLinMomentum(PESO_Object& object) {
	object.objectData.linMomentum.x = object.objectData.linVelocity.x * object.objectData.mass;
	object.objectData.linMomentum.y = object.objectData.linVelocity.y * object.objectData.mass;
	object.objectData.linMomentum.z = object.objectData.linVelocity.z * object.objectData.mass;
};

void PESO_Physics::PESO_CalculateCentreOfMass(PESO_Object& object) {
	double netx = 0, nety = 0, netz = 0;
	double netMass = 0;

	if (object.children.capacity() > 0) {
		for (auto child : object.children) {
			netMass += child.objectData.mass;
			netx += child.objectData.mass * child.objectData.transform.position.x;
			nety += child.objectData.mass * child.objectData.transform.position.y;
			netz += child.objectData.mass * child.objectData.transform.position.z;
		}	
	}
	netMass += object.objectData.mass;
	netx += object.objectData.mass * object.objectData.transform.position.x;
	nety += object.objectData.mass * object.objectData.transform.position.y;
	netz += object.objectData.mass * object.objectData.transform.position.z;

	double COMx = 0, COMy = 0, COMz = 0;

	COMx = netx / netMass;
	COMy = nety / netMass;
	COMz = netz / netMass;

	if (object.children.capacity() > 0) {
		for (auto child : object.children) {
			child.objectData.centreOfMass = Vector3d(COMx, COMy, COMz);
		}
	}
	object.objectData.centreOfMass = Vector3d(COMx, COMy, COMz);
};

void PESO_Physics::PESO_CalculateInertia(PESO_Object& object) {
	object.objectData.inertia = object.objectData.mass * pow(object.objectData.radius, 2);
}

void PESO_Physics::PESO_CalculateOrbitPeriod(PESO_Object& satellite, PESO_Object& target) {
	satellite.objectData.period = sqrt(2 * _PI_ * pow(PESO_CalculateRange(satellite, target), 3) / _UNIVERSAL_CONST_GRAVITATION_ * satellite.objectData.mass);
};

void PESO_Physics::PESO_CalculateReqLinVelocity(PESO_Object& satellite, PESO_Object& target) {
	double reqVelocity = sqrt(_UNIVERSAL_CONST_GRAVITATION_ * target.objectData.mass / PESO_CalculateRange(satellite, target));
	satellite.objectData.reqVelocity = Vector3d(
		(target.objectData.transform.position.x - satellite.objectData.transform.position.x) * reqVelocity,
		(target.objectData.transform.position.y - satellite.objectData.transform.position.y) * reqVelocity,
		(target.objectData.transform.position.z - satellite.objectData.transform.position.z) * reqVelocity
	);
};
#pragma endregion
