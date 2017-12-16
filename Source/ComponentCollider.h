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
