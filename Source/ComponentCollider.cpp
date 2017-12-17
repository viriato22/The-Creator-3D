#include "ComponentCollider.h"
#include "ComponentMeshRenderer.h"
#include "Application.h"
#include "GameObject.h"
#include "Mesh.h"

#include "Bullet\include\btBulletCollisionCommon.h"

#include "MathGeoLib\MathGeoLib.h"
#include "Bullet\include\BulletDynamics\Vehicle\btRaycastVehicle.h"

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

	if (!components.empty())
	{
		for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it);
			enclosingAABB->Enclose(mesh_renderer->GetMesh()->box);
		}

		float3 AABB_size = enclosingAABB->Size();
		shape = new btBoxShape(btVector3(AABB_size.x / 2, AABB_size.y / 2, AABB_size.z / 2));
		size = (float3)(AABB_size.x / 2, AABB_size.y / 2, AABB_size.z / 2);
	}
	else {
		shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
		size = (float3)(0.5f, 0.5f, 0.5f);
	}

	float3 center_point = enclosingAABB->CenterPoint();
	center = new btVector3( center_point.x, center_point.y, center_point.z );

	RELEASE(enclosingAABB);
}

ComponentSphereCollider::ComponentSphereCollider(GameObject* attached_gameobject) : ComponentCollider(attached_gameobject)
{
	SetName("SphereCollider");
	SetType(ComponentType::SphereCollider);

	std::list<Component*> components = attached_gameobject->GetComponents(ComponentType::MeshRenderer);
	AABB* enclosingAABB = new AABB({ 0,0,0 }, { 0,0,0 });

	if (!components.empty())
	{
		for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it);
			enclosingAABB->Enclose(mesh_renderer->GetMesh()->box);
		}

		Sphere enclosing_sphere = enclosingAABB->MinimalEnclosingSphere();
		shape = new btSphereShape(enclosing_sphere.r);
		radius = enclosing_sphere.r;
	}
	else {
		shape = new btSphereShape(0.5f);
		radius = 0.5f;
	}

	float3 center_point = enclosingAABB->CenterPoint();
	center = new btVector3( center_point.x, center_point.y, center_point.z );

	RELEASE(enclosingAABB);
}

ComponentMeshCollider::ComponentMeshCollider(GameObject* attached_gameobject) : ComponentCollider(attached_gameobject)
{
	SetName("MeshCollider");
	SetType(ComponentType::MeshCollider);

	btConvexHullShape* hull = new btConvexHullShape();
	std::list<Component*> components = attached_gameobject->GetComponents(ComponentType::MeshRenderer);

	if (!components.empty())
	{
		for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			Mesh* mesh = ((ComponentMeshRenderer*)(*it))->GetMesh();
			for (int i = 0; (i + 2) < mesh->num_indices; i += 3)
			{
				float vert_x = mesh->vertices[mesh->indices[i]];
				float vert_y = mesh->vertices[mesh->indices[i + 1]];
				float vert_z = mesh->vertices[mesh->indices[i + 2]];

				const btVector3 new_point = { vert_x, vert_y, vert_z };

				hull->addPoint(btVector3(vert_x, vert_y, vert_z));

				// FIX BELOW, EXCEPTION THROWN
				//if(i < 50) hull->addPoint(btVector3(vert_x, vert_y, vert_z));
				//else if (!hull->isInside(new_point, btScalar(0.0f)))	hull->addPoint(btVector3(vert_x, vert_y, vert_z));
			}
		}

		shape = hull;
		btVector3 centroid; btScalar aux;
		hull->getBoundingSphere(centroid, aux);
		center = new btVector3(centroid);
	}
	else
	{
		shape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
		center = new btVector3(0, 0, 0);
		RELEASE(hull);
	}

}


void ComponentCollider::Save(Data & data) const
{
	data.AddInt("Type", GetType());
	data.AddBool("Active", IsActive());
	data.AddUInt("UUID", GetUID());

	// NEEDS IMPLEMENTATION!
}

void ComponentCollider::Load(Data & data)
{
	SetType((Component::ComponentType)data.GetInt("Type"));
	SetActive(data.GetBool("Active"));
	SetUID(data.GetUInt("UUID"));

	// NEEDS IMPLEMENTATION!
}

void ComponentBoxCollider::SetSize(float3 new_size)
{
	delete shape;
	size = new_size;
	shape = new btBoxShape(btVector3(size.x, size.y, size.z));
}

float3 ComponentBoxCollider::GetSize() const
{
	return size;
}

void ComponentSphereCollider::SetRadius(float new_radius)
{
	delete shape;
	radius = new_radius;
	shape = new btSphereShape(radius);
}

float ComponentSphereCollider::GetRadius() const
{
	return radius;
}

ComponentChassisCollider::ComponentChassisCollider(GameObject* attached_gameobject) : ComponentCollider(attached_gameobject)
{
	SetName("WheelCollider");
	SetType(ComponentType::ChassisCollider);

	std::list<Component*> components = attached_gameobject->GetComponents(ComponentType::MeshRenderer);
	AABB* enclosingAABB = new AABB({ 0,0,0 }, { 0,0,0 });
	
	vehicle_raycaster = new btDefaultVehicleRaycaster(App->physics->GetWorld()); //Rodrigo --> for some reason it doesn't let me call App

	if (!components.empty())
	{
		for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		{
			ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)(*it);
			enclosingAABB->Enclose(mesh_renderer->GetMesh()->box);
		}

		float3 AABB_size = enclosingAABB->Size();
		if(attached_gameobject->GetComponent(ComponentType::RigidBody)) shape = new btRaycastVehicle(tuning, attached_gameobject->GetComponent(ComponentType::RigidBody), vehicle_raycaster); //needs a rigidbody to work
		//needs to create an else that creates a rigidbody and creates the vehicle
	}
	else {
		//create a default shape
	}

	float3 center_point = enclosingAABB->CenterPoint();
	center = new btVector3(center_point.x, center_point.y, center_point.z);

	RELEASE(enclosingAABB);
}

void ComponentChassisCollider::GetValues(float& new_mass, float& new_radius, float& new_wheeldamp, float& new_suspensionrate)
{
	new_mass = mass;
	new_radius = radius;
	new_wheeldamp = wheeldamp;
	new_suspensionrate = suspension_rate;
}

void ComponentChassisCollider::SetValues(float new_mass, float new_radius, float new_wheeldamp, float new_suspensionrate, float new_spring, float new_damper)
{
	mass = new_mass;
	radius = new_radius;
	wheeldamp = new_wheeldamp;
	suspension_rate = new_suspensionrate;
	spring = new_spring;
	damper = new_damper;
}