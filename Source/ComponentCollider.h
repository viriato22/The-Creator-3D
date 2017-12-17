#pragma once
#include "Component.h"

class btCollisionShape;
class btCollisionObject;
class btVector3;

class ComponentCollider : public Component
{
public:
	ComponentCollider(GameObject* attached_gameobject);
	~ComponentCollider();

	void OnDataChange() {};

	void Save(Data& data) const;
	void Load(Data& data);

	btCollisionShape* GetShape() { return shape; }
	btVector3* GetCenter() { return center; }
	bool GetTrigger() { return is_trigger; }
	void SetTrigger(bool trigger) { is_trigger = trigger; }

protected:
	btCollisionShape* shape = nullptr;
	btVector3* center = nullptr;

	bool is_trigger = false;
};

class ComponentBoxCollider : public ComponentCollider
{
public:
	ComponentBoxCollider(GameObject* attached_gameobject);

	void SetSize(float3 new_size);
	float3 GetSize() const;

protected:
	float3 size;
};


class ComponentSphereCollider : public ComponentCollider
{
public:
	ComponentSphereCollider(GameObject* attached_gameobject);

	void SetRadius(float new_radius);
	float GetRadius() const;

protected:
	float radius;
};



class ComponentMeshCollider : public ComponentCollider   // right now, only works with first mesh in components     
{
public:
	ComponentMeshCollider(GameObject* attached_gameobject);
};

class ComponentChassisCollider : public ComponentCollider 
{
public:
	ComponentChassisCollider(GameObject* attached_gameobject);

	void GetValues(float& new_mass, float& new_radius, float& new_wheeldamp, float& new_suspensionrate);
	void SetValues(float new_mass, float new_radius, float new_wheeldamp, float new_suspensionrate, float new_spring, float new_damper);

protected:
	//Needed components to create a btraycastvehicle
	btRaycastVehicle::btVehicleTuning tuning;
	btDefaultVehicleRaycaster* vehicle_raycaster = nullptr;

	//Components to modify the behaviour of the car
	float mass; //could be = to rigidbody
	float radius; //m_wheelradius (btwheelinfo)
	float wheeldamp; //m_wheelsDampingCompression (btwheelinfo)
	float suspension_rate; //m_suspensionStiffness (btwheelinfo)
	//can't find anything related to Force App Point Distance maybe delete it from physicswindow
	float spring; //tuning->m_maxSuspensionForce
	float damper; //tuning->m_suspensionDamping
	//can't find anything related to Target Position as it should only be a float maybe delete it from physicswindow
	//the only thing related to slip I found is m_frictionSlip in both tuning and btwheelinfo, should be 
	//5 variables for forward and sideways instead of one https://docs.unity3d.com/ScriptReference/WheelFrictionCurve.html https://forum.unity.com/threads/wheel-friction-curve-revealed.153340/
	//Find a way to convert those 10 values into m_friction slip
};
