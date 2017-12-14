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

	void Save(Data& data) const ;
	void Load(Data& data);

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void GetPos(float* x, float* y, float* z);
	void SetAsSensor(bool is_sensor);
	
	void UpdateGameObjTransform();
	void UpdateRBTransformFromGameObject();

	btRigidBody* getBtRigidBody() { return rb; }

protected:
	btRigidBody* rb;
	ComponentCollider* attached_collider = nullptr;   // rigidbody without collider isn't supported, if so, a collider will be created

	bool is_sensor = false;
};