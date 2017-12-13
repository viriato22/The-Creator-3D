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
	startTransform.setFromOpenGLMatrix(&obj_transform->GetMatrix().v[0][0]);

	btVector3 localInertia(0, 0, 0);
	attached_collider->GetShape()->calculateLocalInertia(INITIAL_RB_MASS, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(INITIAL_RB_MASS, myMotionState, attached_collider->GetShape(), localInertia);

	rb = new btRigidBody(rbInfo);
	rb->setUserPointer(this);

	App->physics->GetWorld()->addRigidBody(rb);

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


