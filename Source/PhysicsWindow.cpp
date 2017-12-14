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
				if (ImGui::Button("Add Collider")) {
					ImGui::OpenPopup("Colliders");
				}
				if (ImGui::BeginPopup("Colliders"))
				{
					if (ImGui::MenuItem("Rigidbody")) {
						if (object1->GetComponent(Component::RigidBody) == nullptr) { //Will leave this one to work as example
							object1->AddComponent(Component::RigidBody);
						}
						else
						{
							CONSOLE_WARNING("GameObject can't have more than 1 Rigidbody!");
						}
					}
					ImGui::EndPopup();
				}

				for (std::list<Component*>::iterator it = object1->components_list.begin(); it != object1->components_list.end(); it++) {
					DrawComponent((*it));
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
			if (ImGui::Button("Make a hinge")) {
				std::list<GameObject*>::iterator ptr;
				ptr = App->scene->selected_gameobjects.begin();
				object1 = (GameObject*)(*ptr);
				ptr++;
				object2 = (GameObject*)(*ptr);
			}
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
	bool FreezePosX, FreezeRotX, FreezePosY, FreezeRotY, FreezePosZ, FreezeRotZ; //these should be changed to a call to their component
	bool gravity, kinematic;
	float mass, drag, angulardrag;

	if (ImGui::CollapsingHeader("Rigidbody")) {
		ImGui::Text("Mass"); ImGui::SameLine();
		ImGui::InputFloat("", &mass);

		ImGui::Text("Drag"); ImGui::SameLine();
		ImGui::InputFloat("", &drag);

		ImGui::Text("Angular Drag"); ImGui::SameLine();
		ImGui::InputFloat("", &angulardrag);

		ImGui::Text("Gravity"); ImGui::SameLine();
		ImGui::Checkbox("", &gravity);

		ImGui::Text("Is kinematic"); ImGui::SameLine();
		ImGui::Checkbox("", &kinematic);

		if (ImGui::CollapsingHeader("Constraints")) {
			ImGui::Text("Freeze position"); ImGui::SameLine();
			ImGui::Checkbox("X", &FreezePosX); ImGui::SameLine();
			ImGui::Checkbox("Y", &FreezePosY); ImGui::SameLine();
			ImGui::Checkbox("Z", &FreezePosZ);

			ImGui::Text("Freeze rotation"); ImGui::SameLine();
			ImGui::Checkbox("X", &FreezeRotX); ImGui::SameLine();
			ImGui::Checkbox("Y", &FreezeRotY); ImGui::SameLine();
			ImGui::Checkbox("Z", &FreezeRotZ);
		}
	}
}