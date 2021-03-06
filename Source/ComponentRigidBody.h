#pragma once
#include "Component.h"

class btRigidBody;
class btMotionState;
class ComponentCollider;
class btTransform;

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
	void UseGravity(bool gravity);
	void SetAsKinematic(bool kinematic);
	void SetAsStatic(bool stat);
	void SetMass(float mass);
	void SetDrag(float drag, float adrag);
	void FreezePos(bool X, bool Y, bool Z);
	void FreezeRot(bool X, bool Y, bool Z);

	bool GetGravity() const;
	bool IsKinematic() const;
	bool IsStatic() const;
	float GetMass() const;
	float GetDrag() const;
	float GetADrag() const;
	void GetFreezePos(bool& X, bool& Y, bool& Z);
	void GetFreezeRot(bool& X, bool& Y, bool& Z);
	
	void UpdateGameObjTransform();
	void UpdateCameraTransform();
	void UpdateRBTransformFromGameObject();
	void ResetMotionState(btTransform* motion_transform);

	btRigidBody* getBtRigidBody() { return rb; }

protected:
	btRigidBody* rb;
	btMotionState* motion_state;
	ComponentCollider* attached_collider = nullptr;   // rigidbody without collider isn't supported, if so, a collider will be created
	float mass = 0.0f;

	bool is_sensor = false;
	bool is_kinematic = false;
	bool is_static = false;

	bool posX = false;
	bool posY = false;
	bool posZ = false;

	bool rotX = false;
	bool rotY = false;
	bool rotZ = false;
};