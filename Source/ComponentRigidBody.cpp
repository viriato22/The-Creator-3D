#include "ComponentRigidBody.h"
#include "ComponentCollider.h"
#include "ComponentMeshRenderer.h"
#include "Application.h"
#include "GameObject.h"

#include "Bullet\include\btBulletDynamicsCommon.h"

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
		if (mesh_renderer)	attached_gameobject->AddComponent(Component::ComponentType::MeshCollider);
		else				attached_gameobject->AddComponent(Component::ComponentType::BoxCollider);
	}


	btTransform startTransform;
	//startTransform.setFromOpenGLMatrix(&sphere.transform);

	//btVector3 localInertia(0, 0, 0);
	//if (mass != 0.f)
	//	colShape->calculateLocalInertia(mass, localInertia);

	//btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	//motions.push_back(myMotionState);
	//btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);

	//btRigidBody* body = new btRigidBody(rbInfo);
	//PhysBody3D* pbody = new PhysBody3D(body);

	//pbody->collision_listeners.push_back(listener);
	//body->setUserPointer(pbody);
	//world->addRigidBody(body);
	//bodies.push_back(pbody);
}

ComponentRigidBody::~ComponentRigidBody()
{
	RELEASE(rb);
}

