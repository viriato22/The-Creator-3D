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
	attached_collider->GetShape()->calculateLocalInertia(INITIAL_RB_MASS, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(INITIAL_RB_MASS, myMotionState, attached_collider->GetShape(), localInertia);

	rb = new btRigidBody(rbInfo);
	rb->setUserPointer(this);

	App->physics->GetWorld()->addRigidBody(rb);
	App->physics->GetRigidBodies()->push_back(rb);

	//delete myMotionState;
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

	rb->getMotionState()->setWorldTransform(t);
	Push(0,1,0);
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

void ComponentRigidBody::SetAsSensor(bool is_sensor)
{
	if (is_sensor != is_sensor)
	{
		is_sensor = is_sensor;
		if (is_sensor == true)
			rb->setCollisionFlags(rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			rb->setCollisionFlags(rb->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}



