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
			if (ImGui::Button("Select GameObject")) {
				if (object1 == nullptr) {
					object1 = App->scene->selected_gameobjects.front();
				}
				else if (object2 == nullptr && App->scene->selected_gameobjects.front() != object1) {
					object2 = App->scene->selected_gameobjects.front();
				}
				else {
					//error message
				}
			}

			for (std::list<Component*>::iterator it = object1->components_list.begin(); it != object1->components_list.end(); it++) {
				DrawComponent((*it));
				ImGui::Separator();
				ImGui::Spacing();
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
	if (ImGui::Button("Make Rigidbody")) {

	}
}