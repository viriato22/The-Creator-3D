#include "PhysicsWindow.h"
#include "Application.h"
#include "GameObject.h"
#include "TagsAndLayers.h"
#include "ModuleEditor.h"
#include "TagsAndLayersWindow.h"
#include "ModuleScene.h"
#include "imgui/CustomImGui.h"
#include "ModuleRenderer3D.h"
#include "ComponentRigidBody.h"
#include "ComponentCollider.h"

PhysicsWindow::PhysicsWindow()
{
	active = true;
	window_name = "Physics";
}

PhysicsWindow::~PhysicsWindow()
{
}

void PhysicsWindow::DrawWindow()
{
	if (ImGui::BeginDock(window_name.c_str(), false, false, App->IsPlaying(), ImGuiWindowFlags_HorizontalScrollbar)) {
		GameObject* object1 = nullptr;
		GameObject* object2 = nullptr;
		if (App->scene->selected_gameobjects.size() == 1) {
			object1 = App->scene->selected_gameobjects.front();
			if (object1 != nullptr && !object1->is_on_destroy) {
				for (std::list<Component*>::iterator it = object1->components_list.begin(); it != object1->components_list.end(); it++) {
					DrawComponent((*it));
				}

				if (ImGui::Button("Add Collider")) {
					ImGui::OpenPopup("Colliders");
				}
				if (ImGui::BeginPopup("Colliders"))
				{
					if (ImGui::MenuItem("Rigidbody")) {
						if (object1->GetComponent(Component::RigidBody) == nullptr) {
							object1->AddComponent(Component::RigidBody);
						}
						else
						{
							CONSOLE_WARNING("GameObject can't have more than 1 Rigidbody!");
						}
						if (object1->GetComponent(Component::BoxCollider) == nullptr) {
							object1->AddComponent(Component::BoxCollider);
						}
						else
						{
							CONSOLE_WARNING("GameObject can't have more than 1 Box Collider!");
						}
						if (object1->GetComponent(Component::SphereCollider) == nullptr) {
							object1->AddComponent(Component::SphereCollider);
						}
						else
						{
							CONSOLE_WARNING("GameObject can't have more than 1 Sphere Collider!");
						}
					}
					ImGui::EndPopup();
				}
			}
		}
		else if (App->scene->selected_gameobjects.size() == 2) {
			//Maybe make this only accessible if they are not alredy constrained between eachother
			if (ImGui::Button("Make constraint")) {
				std::list<GameObject*>::iterator ptr; //This code is to select each gameobject
				ptr = App->scene->selected_gameobjects.begin();
				object1 = (GameObject*)(*ptr);
				ptr++;
				object2 = (GameObject*)(*ptr);
				
			}
			/*if (ImGui::Button("Make a hinge")) {
				std::list<GameObject*>::iterator ptr;
				ptr = App->scene->selected_gameobjects.begin();
				object1 = (GameObject*)(*ptr);
				ptr++;
				object2 = (GameObject*)(*ptr);
			}*/
		}
		else {

		}
	}
	ImGui::EndDock();
}

void PhysicsWindow::DrawComponent(Component * component)
{
	switch (component->GetType())
	{
	case Component::RigidBody:
		DrawRigidbodyPanel((ComponentRigidBody*)component);
		break;
	
	default:
		break;
	}
}

void PhysicsWindow::DrawRigidbodyPanel(ComponentRigidBody* rigidbody) {
	bool FreezePosX, FreezeRotX, FreezePosY, FreezeRotY, FreezePosZ, FreezeRotZ;
	bool gravity, kinematic;
	float mass, drag, angulardrag;

	rigidbody->GetFreezePos(FreezePosX, FreezePosY, FreezePosZ);
	rigidbody->GetFreezeRot(FreezeRotX, FreezeRotY, FreezeRotZ);
	gravity = rigidbody->GetGravity();
	kinematic = rigidbody->IsKinematic();
	mass = rigidbody->GetMass();
	drag = rigidbody->GetDrag();
	angulardrag = rigidbody->GetADrag();

	if (ImGui::CollapsingHeader("Rigidbody")) {
		if (ImGui::DragFloat("Mass", &mass, 0.25f))
		{
			rigidbody->SetMass(mass);
		}

		if (ImGui::DragFloat("Drag", &drag, 0.25f)) {
			rigidbody->SetDrag(drag, angulardrag);
		}

		if (ImGui::DragFloat("Angular Drag", &angulardrag, 0.25f)) {
			rigidbody->SetDrag(drag, angulardrag);
		}

		if (ImGui::Checkbox("Use gravity", &gravity)) {
			rigidbody->UseGravity(gravity);
		}

		if (ImGui::Checkbox("Is kinematic", &kinematic)) {
			rigidbody->SetAsKinematic(kinematic);
		}

		if (ImGui::CollapsingHeader("Constraints")) {
			ImGui::Text("Freeze position"); ImGui::SameLine();
			if (ImGui::Checkbox("X", &FreezePosX)) {
				rigidbody->FreezePos(FreezePosX, FreezePosY, FreezePosZ);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Y", &FreezePosY)) {
				rigidbody->FreezePos(FreezePosX, FreezePosY, FreezePosZ);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Z", &FreezePosZ)) {
				rigidbody->FreezePos(FreezePosX, FreezePosY, FreezePosZ);
			}

			ImGui::Spacing();

			ImGui::Text("Freeze rotation"); ImGui::SameLine();
			if (ImGui::Checkbox("X", &FreezeRotX)) {
				rigidbody->FreezeRot(FreezeRotX, FreezeRotY, FreezeRotZ);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Y", &FreezeRotY)) {
				rigidbody->FreezeRot(FreezeRotX, FreezeRotY, FreezeRotZ);
			}
			ImGui::SameLine();
			if (ImGui::Checkbox("Z", &FreezeRotZ)) {
				rigidbody->FreezeRot(FreezeRotX, FreezeRotY, FreezeRotZ);
			}
		}
	}
}

void PhysicsWindow::DrawBoxColliderPanel(ComponentBoxCollider* boxcollider)
{
	bool trigger;
	float3 center, size;

	trigger = boxcollider->GetTrigger();
	//center = (float3)boxcollider->GetCenter();
	size = boxcollider->GetSize();

	if (ImGui::CollapsingHeader("Box Collider")) {
		if (ImGui::Checkbox("Is Trigger", &trigger))
		{
			boxcollider->SetTrigger(trigger);
		}

		ImGui::Spacing();

		if (ImGui::DragFloat3("Center", (float*)&center, 0.25f))
		{

		}
		if (ImGui::DragFloat3("Size", (float*)&size, 0.25f))
		{
			boxcollider->SetSize(size);
		}
	}
}

void PhysicsWindow::DrawSphereColliderPanel(ComponentSphereCollider* spherecollider)
{
	bool trigger;
	float3 center;
	float radius;

	if (ImGui::CollapsingHeader("Sphere Collider")) {
		ImGui::Checkbox("Is Trigger", &trigger);

		ImGui::Spacing();

		ImGui::DragFloat3("Center", (float*)&center, 0.25f);
		ImGui::DragFloat("Radius", &radius, 0.25f);
	}
}

void PhysicsWindow::DrawCapsuleColliderPanel()
{
	bool trigger;
	float3 center;
	float radius, height;

	if (ImGui::CollapsingHeader("Capsule Collider")) {
		ImGui::Checkbox("Is Trigger", &trigger);

		ImGui::Spacing();

		ImGui::DragFloat3("Center", (float*)&center, 0.25f);
		ImGui::DragFloat("Radius", &radius, 0.25f);
		ImGui::DragFloat("Height", &height, 0.25f);
	}
}

void PhysicsWindow::DrawWheelColliderPanel()
{
	float mass, radius, wheeldamp, susprate, forceapp;
	float3 center;
	float spring, damper, targetpos;
	float ffexslip, ffexval, ffaslip, ffasval, ffstiffness;
	float sfexslip, sfexval, sfaslip, sfasval, sfstiffness;

	if (ImGui::CollapsingHeader("Wheel Collider")) {
		ImGui::DragFloat("Mass", &mass, 0.25f);
		ImGui::DragFloat("Radius", &radius, 0.25f);
		ImGui::DragFloat("Wheel Damping Rate", &wheeldamp, 0.25f);
		ImGui::DragFloat("Suspension Rate", &susprate, 0.05f);
		ImGui::DragFloat("Force App Point Distance", &forceapp, 0.25f);

		ImGui::DragFloat3("Center", (float*)&center, 0.25f);

		if (ImGui::CollapsingHeader("Suspension Spring")) {
			ImGui::DragFloat("Spring", &spring, 0.25f);
			ImGui::DragFloat("Damper", &damper, 0.25f);
			ImGui::DragFloat("Target Position", &targetpos, 0.05f);
		}

		if (ImGui::CollapsingHeader("Forward Friction")) {
			ImGui::DragFloat("Extremum Slip", &ffexslip, 0.05f);
			ImGui::DragFloat("Extremum Value", &ffexval, 0.05f);
			ImGui::DragFloat("Asymptote Slip", &ffaslip, 0.05f);
			ImGui::DragFloat("Asymptote Value", &ffasval, 0.05f);
			ImGui::DragFloat("Stiffness", &ffstiffness, 0.1f);
		}

		if (ImGui::CollapsingHeader("Sideways Friction")) {
			ImGui::DragFloat("Extremum Slip", &sfexslip, 0.05f);
			ImGui::DragFloat("Extremum Value", &sfexval, 0.05f);
			ImGui::DragFloat("Asymptote Slip", &sfaslip, 0.05f);
			ImGui::DragFloat("Asymptote Value", &sfasval, 0.05f);
			ImGui::DragFloat("Stiffness", &sfstiffness, 0.1f);
		}
	}
}

//void PhysicsWindow::DrawHingeConstraintPanel()
//{
//	if (ImGui::CollapsingHeader("Hinge Constraint")) {
//
//	}
//}
//
void PhysicsWindow::DrawFixedConstraintPanel()
{
	if (ImGui::CollapsingHeader("Fixed Constraint")) {

	}
}
//
//void PhysicsWindow::DrawSpringConstraintPanel()
//{
//	if (ImGui::CollapsingHeader("Spring Constraint")) {
//
//	}
//}

//void CreateConstrint() {
//	btPoint2PointConstraint(btRigidBody& rbA,
//		btRigidBody& rbB,
//		const btVector3& pivotInA,
//		const btVector3& pivotInB);
//}