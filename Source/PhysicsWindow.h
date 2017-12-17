#pragma once

#include "Window.h"

class Component;
class ComponentRigidBody;
class ComponentBoxCollider;
class ComponentSphereCollider;

class PhysicsWindow :
	public Window
{
public:
	PhysicsWindow();
	virtual ~PhysicsWindow();

	void DrawWindow();

	void DrawComponent(Component * component);

	void DrawRigidbodyPanel(ComponentRigidBody* rigidbody);
	void DrawBoxColliderPanel(ComponentBoxCollider* boxcollider);
	void DrawSphereColliderPanel(ComponentSphereCollider* spherecollider);
	void DrawCapsuleColliderPanel();
	void DrawWheelColliderPanel();

	//void DrawHingeConstraintPanel();
	void DrawFixedConstraintPanel();
	//void DrawSpringConstraintPanel();

private:
	//void SelectObjects(GameObject* object1, GameObject* object2); //For some reason it doesn't work
};