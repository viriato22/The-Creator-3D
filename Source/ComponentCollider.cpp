#include "ComponentCollider.h"
#include "ComponentMeshRenderer.h"
#include "Application.h"
#include "GameObject.h"
#include "Mesh.h"

#include "Bullet\include\btBulletCollisionCommon.h"

#include "MathGeoLib\MathGeoLib.h"

ComponentCollider::ComponentCollider(GameObject* attached_gameobject)
{
	SetActive(true);
	SetGameObject(attached_gameobject);
	SetUID(App->RandomNumber().Int());
}

ComponentCollider::~ComponentCollider()
{
	RELEASE(shape);
	RELEASE(center);
}

ComponentBoxCollider::ComponentBoxCollider(GameObject* attached_gameobject) : ComponentCollider(attached_gameobject)
{
	SetName("BoxCollider");
	SetType(ComponentType::BoxCollider);

	std::list<Component*> components = attached_gameobject->GetComponents(ComponentType::MeshRenderer);
	AABB* enclosingAABB = new AABB({ 0,0,0 }, { 0,0,0 });

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it);
		enclosingAABB->Enclose(mesh_renderer->GetMesh()->box);
	}

	float3 center_point = enclosingAABB->CenterPoint();
	center = new btVector3( center_point.x, center_point.y, center_point.z );

	float3 AABB_size = enclosingAABB->Size();
	shape = new btBoxShape(btVector3(AABB_size.x / 2, AABB_size.y / 2, AABB_size.z / 2));
}

ComponentSphereCollider::ComponentSphereCollider(GameObject* attached_gameobject) : ComponentCollider(attached_gameobject)
{
	SetName("SphereCollider");
	SetType(ComponentType::SphereCollider);

	std::list<Component*> components = attached_gameobject->GetComponents(ComponentType::MeshRenderer);
	AABB* enclosingAABB = new AABB({ 0,0,0 }, { 0,0,0 });

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it);
		enclosingAABB->Enclose(mesh_renderer->GetMesh()->box);
	}

	float3 center_point = enclosingAABB->CenterPoint();
	center = new btVector3( center_point.x, center_point.y, center_point.z );

	Sphere enclosing_sphere = enclosingAABB->MinimalEnclosingSphere();
	shape = new btSphereShape(enclosing_sphere.r);
}

ComponentMeshCollider::ComponentMeshCollider(GameObject* attached_gameobject) : ComponentCollider(attached_gameobject)
{
	SetName("MeshCollider");
	SetType(ComponentType::MeshCollider);

	btCompoundShape* compound = new btCompoundShape();
	std::list<Component*> components = attached_gameobject->GetComponents(ComponentType::MeshRenderer);

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
	{
		Mesh* mesh = ((ComponentMeshRenderer*)(*it))->GetMesh();
		for (int i = 0; i < mesh->num_indices * 3; i += 3)
		{
			btConvexHullShape triangle;
			float vert_x = mesh->vertices[mesh->indices[i]];
			float vert_y = mesh->vertices[mesh->indices[i + 1]];
			float vert_z = mesh->vertices[mesh->indices[i + 2]];

			triangle.addPoint(btVector3(vert_x, vert_y, vert_z), false);
			btTransform transform;
			transform.setIdentity();

			compound->addChildShape(transform, &triangle);
		}
	}

	center = new btVector3( 0,0,0 );
	shape = compound;
}