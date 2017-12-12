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

	void Save(Data& data) const {};
	void Load(Data& data) {};

protected:
	btCollisionShape* shape = nullptr;
	btCollisionObject* collision_obj = nullptr;
	btVector3* center = nullptr;
};

class ComponentBoxCollider : public ComponentCollider
{
public:
	ComponentBoxCollider(GameObject* attached_gameobject);
};


class ComponentSphereCollider : public ComponentCollider
{
public:
	ComponentSphereCollider(GameObject* attached_gameobject);
};



class ComponentMeshCollider : public ComponentCollider   // right now, only works with first mesh in components     
{
public:
	ComponentMeshCollider(GameObject* attached_gameobject);
};
