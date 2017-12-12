#include "PhysicsWindow.h"
#include "Application.h"
#include "GameObject.h"
#include "TagsAndLayers.h"
#include "ModuleEditor.h"
#include "TagsAndLayersWindow.h"
#include "ModuleScene.h"
#include "imgui/CustomImGui.h"
#include "ModuleRenderer3D.h"

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
		GameObject* selected_gameobject = nullptr;
		if (App->scene->selected_gameobjects.size() == 1) {
			selected_gameobject = App->scene->selected_gameobjects.front();
		}
		else if (App->scene->selected_gameobjects.size() == 2) {
			//Should give possibility to make hinges
		}
	}
	ImGui::EndDock();
}