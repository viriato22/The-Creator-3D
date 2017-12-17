#include "ComponentRigidBody.h"
#include "ComponentCollider.h"
#include "ComponentTransform.h"
#include "ComponentMeshRenderer.h"
#include "ModulePhysics3D.h"
#include "Application.h"
#include "GameObject.h"

#include "Bullet\include\btBulletDynamicsCommon.h"

#define INITIAL_RB_MASS 1.0f

ComponentRigidBody::ComponentRigidBody(GameObject* attached_gameobject)
{
	SetActive(true);
	SetGameObject(attached_gameobject);

	SetName("RigidBody");
	SetType(ComponentType::RigidBody);
	SetUID(App->RandomNumber().Int());
	
	std::list<Component*> colliders = attached_gameobject->GetColliders();

	if (!colliders.empty()) attached_collider = (ComponentCollider*)colliders.front();
	else
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)attached_gameobject->GetComponent(Component::ComponentType::MeshRenderer);
		if (mesh_renderer)	attached_collider = (ComponentCollider*)attached_gameobject->AddComponent(Component::ComponentType::MeshCollider);
		else				attached_collider = (ComponentCollider*)attached_gameobject->AddComponent(Component::ComponentType::BoxCollider);
	}

	ComponentTransform* obj_transform = (ComponentTransform*)attached_gameobject->GetComponent(ComponentType::Transform);
	btTransform startTransform;

	float3 position = obj_transform->GetGlobalPosition();
	startTransform.setOrigin({ position.x, position.y, position.z });

	float3 rotation = obj_transform->GetGlobalRotation();
	btQuaternion quat; quat.setEuler(rotation.y, rotation.x, rotation.z);
	startTransform.setRotation(quat);

	btVector3 localInertia(0, 0, 0);
	mass = INITIAL_RB_MASS;
	attached_collider->GetShape()->calculateLocalInertia(mass, localInertia);

	motion_state = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motion_state, attached_collider->GetShape(), localInertia);

	rb = new btRigidBody(rbInfo);
	rb->setUserPointer(this);

	App->physics->GetWorld()->addRigidBody(rb);
	App->physics->GetRigidBodies()->push_back(rb);

	if (attached_gameobject->IsStatic())
		SetAsStatic(true);
}

ComponentRigidBody::~ComponentRigidBody()
{
	App->physics->GetWorld()->removeRigidBody(rb);
	RELEASE(rb);
}

void ComponentRigidBody::Save(Data & data) const
{
	data.AddInt("Type", GetType());
	data.AddBool("Active", IsActive());
	data.AddUInt("UUID", GetUID());

	// NEEDS IMPLEMENTATION!
}

void ComponentRigidBody::Load(Data & data)
{
	SetType((Component::ComponentType)data.GetInt("Type"));
	SetActive(data.GetBool("Active"));
	SetUID(data.GetUInt("UUID"));

	// NEEDS IMPLEMENTATION!
}

void ComponentRigidBody::Push(float x, float y, float z)
{
	rb->applyCentralImpulse(btVector3(x, y, z));
}


void ComponentRigidBody::GetTransform(float* matrix) const
{
	if (rb != NULL && matrix != NULL)
	{
		btTransform t;
		rb->getMotionState()->getWorldTransform(t);
		t.getOpenGLMatrix(matrix);
	}
}


void ComponentRigidBody::SetTransform(const float* matrix) const
{
	if (rb != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		rb->getMotionState()->setWorldTransform(t);
	}
}

void ComponentRigidBody::UpdateGameObjTransform()
{
	btTransform t;
	rb->getMotionState()->getWorldTransform(t);
	ComponentTransform* obj_transform = (ComponentTransform*)GetGameObject()->GetComponent(Component::ComponentType::Transform);

	btVector3 pos = t.getOrigin();
	obj_transform->SetPosition({pos.x(), pos.y(), pos.z()});

	btQuaternion quat = t.getRotation();
	obj_transform->SetRotation({ quat.getX(), quat.getY(), quat.getZ() });

	obj_transform->UpdateGlobalMatrix();
}

void ComponentRigidBody::UpdateRBTransformFromGameObject()
{
	ComponentTransform* obj_transform = (ComponentTransform*)GetGameObject()->GetComponent(ComponentType::Transform);
	btTransform t;

	float3 position = obj_transform->GetGlobalPosition();
	t.setOrigin({ position.x, position.y, position.z });

	float3 rotation = obj_transform->GetGlobalRotation();
	btQuaternion quat; quat.setEuler(rotation.y, rotation.x, rotation.z);
	t.setRotation(quat);

	ResetMotionState(&t);
}

void ComponentRigidBody::ResetMotionState(btTransform* motion_transform)
{
	delete motion_state;
	motion_state = new btDefaultMotionState(*motion_transform);

	rb->setMotionState(motion_state);
	rb->activate(true);

}

void ComponentRigidBody::SetPos(float x, float y, float z)
{
	btTransform t;
	rb->getMotionState()->getWorldTransform(t);
	t.setOrigin(btVector3(x, y, z));
	rb->setWorldTransform(t);
}

void ComponentRigidBody::GetPos(float* x, float* y, float* z)
{
	btTransform t;
	rb->getMotionState()->getWorldTransform(t);
	btVector3 pos = t.getOrigin();
	btVector3FloatData posFloat;
	pos.serializeFloat(posFloat);
	*x = posFloat.m_floats[0];
	*y = posFloat.m_floats[1];
	*z = posFloat.m_floats[2];
}

void ComponentRigidBody::SetAsSensor(bool sensor)
{
	if (is_sensor != sensor)
	{
		is_sensor = sensor;
		if (is_sensor == true)
			rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			rb->setCollisionFlags(rb->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

void ComponentRigidBody::UseGravity(bool gravity)
{
	if(gravity) rb->setGravity(GRAVITY);
	else		rb->setGravity(btVector3(0, 0, 0));

	btTransform t;
	rb->getMotionState()->getWorldTransform(t);
	ResetMotionState(&t);
}

void ComponentRigidBody::SetAsKinematic(bool kinematic)
{
	if (is_kinematic != kinematic)
	{
		is_kinematic = kinematic;
		if (is_kinematic == true)
			rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		else
			rb->setCollisionFlags(rb->getCollisionFlags() &~btCollisionObject::CF_KINEMATIC_OBJECT);

		btTransform t;
		rb->getMotionState()->getWorldTransform(t);
		ResetMotionState(&t);
	}
}

void ComponentRigidBody::SetAsStatic(bool stat)
{
	if (is_static != stat)
	{
		is_static = stat;
		if (is_static == true)
			rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
		else
			rb->setCollisionFlags(rb->getCollisionFlags() &~btCollisionObject::CF_STATIC_OBJECT);

		btTransform t;
		rb->getMotionState()->getWorldTransform(t);
		ResetMotionState(&t);
	}
}

void ComponentRigidBody::SetMass(float new_mass)
{
	mass = new_mass;
	rb->setMassProps(mass, rb->getLocalInertia()); 

	btTransform t;
	rb->getMotionState()->getWorldTransform(t);
	ResetMotionState(&t);
}

void ComponentRigidBody::SetDrag(float drag, float adrag)
{
	rb->setDamping(drag, adrag);

	btTransform t;
	rb->getMotionState()->getWorldTransform(t);
	ResetMotionState(&t);
}

bool ComponentRigidBody::GetGravity() const
{
	if (rb->getGravity().isZero())	return false;
	else							return true;
}

bool ComponentRigidBody::IsKinematic() const
{
	return is_kinematic;
}

bool ComponentRigidBody::IsStatic() const
{
	return is_static;
}

float ComponentRigidBody::GetMass() const
{
	return mass;
}

float ComponentRigidBody::GetDrag() const
{
	return rb->getLinearDamping();
}

float ComponentRigidBody::GetADrag() const
{
	return rb->getAngularDamping();
}

void ComponentRigidBody::FreezePos(bool X, bool Y, bool Z)
{
	posX = X;
	posY = Y;
	posZ = Z;
}

void ComponentRigidBody::FreezeRot(bool X, bool Y, bool Z)
{
	rotX = X;
	rotY = Y;
	rotZ = Z;
}

void ComponentRigidBody::GetFreezePos(bool& X, bool& Y, bool& Z)
{
	X = posX;
	Y = posY;
	Z = posZ;
}

void ComponentRigidBody::GetFreezeRot(bool& X, bool& Y, bool& Z)
{
	X = rotX;
	Y = rotY;
	Z = rotZ;
}