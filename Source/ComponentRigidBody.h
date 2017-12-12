#pragma once
#include "Component.h"

class btRigidBody;
class ComponentCollider;

class ComponentRigidBody : public Component
{
public:
	ComponentRigidBody(GameObject* attached_gameobject);
	~ComponentRigidBody();

	void OnDataChange() {};

	void Save(Data& data) const {};
	void Load(Data& data) {};

protected:
	btRigidBody* rb;
	ComponentCollider* attached_collider = nullptr;   // rigidbody without collider isn't supported, if so, a collider will be created

};