#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentRigidBody.h"
#include "ComponentCollider.h"
#include "ComponentPhysics.h"
#include "Mesh.h"
#include "ModuleResources.h"

GameObject::GameObject(GameObject* parent)
{
	active = true;
	this->parent = nullptr;
	if (parent != nullptr)
	{
		SetParent(parent);
	}
	else
	{
		is_root = true;
	}
	tag = "Default";
	layer = "Default";
	name = "New GameObject";
	is_on_destroy = false;
	is_selected = false;
	is_static = false;
	is_used_in_prefab = false;
	AddComponent(Component::Transform);

	uuid = App->RandomNumber().Int();
}

GameObject::~GameObject()
{
	App->resources->RemoveGameObject(this);
	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); ++it) {
		RELEASE(*it);
	}

	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it) {
		RELEASE(*it);
	}
}

Component * GameObject::AddComponent(Component::ComponentType component_type)
{
	Component* component = nullptr;

	switch (component_type)
	{
	case Component::Transform:
		components_list.push_back(component = new ComponentTransform(this));
		break;
	case Component::Camera:
		components_list.push_back(component = new ComponentCamera(this));
		break;
	case Component::RigidBody:
		components_list.push_back(component = new ComponentRigidBody(this));
		break;
	case Component::MeshRenderer:
		components_list.push_back(component = new ComponentMeshRenderer(this));
		break;
	case Component::BoxCollider:
		components_list.push_back(component = new ComponentBoxCollider(this));
		break;
	case Component::SphereCollider:
		components_list.push_back(component = new ComponentSphereCollider(this));
		break;
	case Component::MeshCollider:
		components_list.push_back(component = new ComponentMeshCollider(this));
		break;
	case Component::AudioSource:
		break;
	case Component::Animaton:
		break;
	case Component::Script:
		break;
	case Component::ParticleSystem:
		break;
	case Component::Physics:
		components_list.push_back(component = new ComponentPhysics(this));
		break;
	default:
		break;
	}

	return component;
}

Component * GameObject::GetComponent(Component::ComponentType component_type)
{
	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); it++) {
		if ((*it)->GetType() == component_type) {
			return (*it);
		}
	}
	return nullptr;
}

std::list<Component*> GameObject::GetComponents(Component::ComponentType component_type)
{
	std::list<Component*> components;

	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); it++) 
	{
		if ((*it)->GetType() == component_type)  components.push_back(*it);
	}

	return components;
}


std::list<Component*> GameObject::GetColliders()
{
	std::list<Component*> components;

	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); it++)
	{
		if ((*it)->GetType() == Component::ComponentType::BoxCollider ||
			(*it)->GetType() == Component::ComponentType::SphereCollider ||
			(*it)->GetType() == Component::ComponentType::MeshCollider)  components.push_back(*it);
	}

	return components;
}


Component * GameObject::GetComponent(std::string component_type)
{
	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); it++) {
		if ((*it)->GetName() == component_type) {
			return (*it);
		}
	}
	return nullptr;
}

void GameObject::DestroyComponent(Component::ComponentType component)
{
	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end();) {
		if ((*it)->GetType() == component) {
			RELEASE(*it);
			it = components_list.erase(it);
		}
		else {
			it++;
		}
	}
}

void GameObject::AddChild(GameObject * gameobject)
{
	childs.push_back(gameobject);
}

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(bool active)
{
	this->active = active;
	if (is_static)
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)GetComponent(Component::MeshRenderer);
		if (mesh_renderer != nullptr)
		{
			if (active)
			{
				App->scene->InsertGoInOctree(mesh_renderer);
			}
			else
			{
				App->scene->EraseGoInOctree(mesh_renderer);
			}
		}
	}
}

void GameObject::SetStatic(bool is_static)
{
	this->is_static = is_static;
	if (active)
	{
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)GetComponent(Component::MeshRenderer);
		if (mesh_renderer != nullptr)
		{
			if (is_static)
			{
				App->scene->InsertGoInOctree(mesh_renderer);
				App->scene->static_meshes.push_back(mesh_renderer);
			}
			else
			{
				App->scene->EraseGoInOctree(mesh_renderer);
				App->scene->static_meshes.remove(mesh_renderer);
			}
		}
	}
}

bool GameObject::IsStatic() const
{
	return is_static;
}

bool GameObject::IsSelected() const
{
	return is_selected;
}

void GameObject::SetSelected(bool selected)
{
	is_selected = selected;
}

bool GameObject::IsRoot() const
{
	return is_root;
}

void GameObject::SetRoot(bool root)
{
	is_root = root;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject * parent)
{
	if (this->parent == parent) {
		return;
	}

	if (this->parent != nullptr) {
		this->parent->childs.remove(this);
	}

	this->parent = parent;
	this->parent->childs.push_back(this);

	if (is_root)
	{
		is_root = false;
		App->scene->root_gameobjects.remove(this);
	}
}

void GameObject::SetName(std::string name)
{
	this->name = name;
}

std::string GameObject::GetName() const
{
	return name;
}

void GameObject::SetTag(std::string tag)
{
	this->tag = tag;
}

std::string GameObject::GetTag() const
{
	return tag;
}

void GameObject::SetLayer(std::string layer)
{
	this->layer = layer;
}

std::string GameObject::GetLayer() const
{
	return layer;
}

void GameObject::SetIsUsedInPrefab(bool used)
{
	is_used_in_prefab = used;
}

bool GameObject::GetIsUsedInPrefab() const
{
	return is_used_in_prefab;
}

int GameObject::GetAllChildsCount() const
{
	int count = childs.size();
	for (std::list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); it++)
	{
		count += (*it)->GetAllChildsCount();
	}

	return count;
}

void GameObject::GetAllChildsName(std::vector<std::string>& names)
{
	for (std::list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->GetAllChildsName(names);
		names.push_back((*it)->name);
	}
}

void GameObject::GetAllChildsMeshesNames(std::vector<std::string>& names)
{
	for (std::list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->GetAllChildsName(names);
		ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)GetComponent(Component::MeshRenderer);
		if (mesh_renderer != nullptr)
		{
			names.push_back((*it)->name);
		}
	}
}

void GameObject::UpdateBoundingBox()
{
	ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)GetComponent(Component::MeshRenderer);
	if (mesh_renderer != nullptr)
	{
		mesh_renderer->UpdateBoundingBox();
	}
}

math::float4x4 GameObject::GetGlobalTransfomMatrix()
{
	ComponentTransform* transform = (ComponentTransform*)GetComponent(Component::Transform);
	return transform->GetMatrix();
}

const float * GameObject::GetOpenGLMatrix()
{
	ComponentTransform* transform = (ComponentTransform*)GetComponent(Component::Transform);
	return transform->GetOpenGLMatrix();
}

void GameObject::UpdateGlobalMatrix()
{
	ComponentTransform* transform = (ComponentTransform*)GetComponent(Component::Transform);
	transform->UpdateGlobalMatrix();
}

void GameObject::UpdateCamera()
{
	ComponentCamera* camera = (ComponentCamera*)GetComponent(Component::Camera);
	if (camera != nullptr)
	{
		camera->UpdatePosition();
	}
}

void GameObject::SetGlobalTransfomMatrix(const float4x4 & matrix)
{
	ComponentTransform* transform = (ComponentTransform*)GetComponent(Component::Transform);
	transform->SetMatrix(matrix);
}

void GameObject::SetParentByID(UID parent_id)
{
	SetParent(App->scene->FindGameObject(parent_id));
}

void GameObject::Destroy()
{
	App->scene->AddGameObjectToDestroy(this);
}

void GameObject::OnDestroy()
{
	is_on_destroy = true;

	std::list<GameObject*>::iterator it = find(App->scene->selected_gameobjects.begin(), App->scene->selected_gameobjects.end(), this);
	if (it != App->scene->selected_gameobjects.end()) {
		App->scene->selected_gameobjects.erase(it);
		is_selected = false;
	}

	std::list<GameObject*>::iterator it2 = find(App->scene->scene_gameobjects.begin(), App->scene->scene_gameobjects.end(), this);
	if (it2 != App->scene->scene_gameobjects.end()) {
		App->scene->scene_gameobjects.erase(it2);
	}
	App->resources->RemoveGameObject(this);

	ComponentMeshRenderer* mesh_renderer = (ComponentMeshRenderer*)GetComponent(Component::MeshRenderer);
	if (mesh_renderer) mesh_renderer->UnloadFromMemory();
	
	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end();) {
		if (*it != nullptr) {
			(*it)->OnDestroy();
			CONSOLE_DEBUG("GameObject Destroyed: %s", (*it)->GetName().c_str());
			RELEASE(*it);
			it = childs.erase(it);
		}
	}

	if (parent != nullptr && !parent->is_on_destroy) {
		parent->childs.remove(this);
	}
	
}

UID GameObject::GetUID() const
{
	return uuid;
}


void GameObject::Save(Data & data, bool is_duplicated)
{
	std::string tempName = name;  //<- needed if is a duplicated game_object
	if (is_duplicated) {
		App->scene->RenameDuplicatedGameObject(this);
	}

	data.CreateSection("GameObject_" + std::to_string(App->scene->saving_index++));
	uint new_uuid = uuid;
	bool temp_active = active;
	bool temp_static = is_static;
	bool temp_selected = is_selected;
	std::string temp_tag = tag;
	std::string temp_layer = layer;
	if (is_duplicated)
	{
		uuid = App->RandomNumber().Int();
		active = true;
		is_static = false;
		is_selected = false;
		tag = "Default";
		layer = "Default";
	}
	data.AddUInt("UUID", uuid);
	data.AddString("Name", name);
	data.AddString("Tag", tag);
	data.AddString("Layer", layer);
	data.AddBool("Active", active);

	data.CreateSection("Components");
	data.AddInt("Components_Count", components_list.size());
	int componentIndex = 0;
	for (std::list<Component*>::const_iterator it = components_list.begin(); it != components_list.end(); it++) {
		data.CreateSection("Component_" + std::to_string(componentIndex));
		(*it)->Save(data);
		data.CloseSection();
		componentIndex++;
	}
	data.CloseSection();

	data.AddUInt("ParentID", parent ? parent->GetUID() : 0);
	data.AddBool("IsRoot", is_root);
	data.AddBool("IsStatic", is_static);
	data.CloseSection();

	//Save all childs recursively
	for (std::list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); it++) {
		(*it)->Save(data, is_duplicated);
	}

	name = tempName;
	uuid = new_uuid;
	active = temp_active;
	is_static = temp_static;
	is_selected = temp_selected;
	tag = temp_tag;
	layer = temp_layer;
}

void GameObject::Load(Data & data, bool is_prefab)
{
	uuid = data.GetUInt("UUID");
	name = data.GetString("Name");
	tag = data.GetString("Tag");
	layer = data.GetString("Layer");
	active = data.GetBool("Active");
	data.EnterSection("Components");
	int componentsCount = data.GetInt("Components_Count");
	for (int i = 0; i < componentsCount; i++)
	{
		data.EnterSection("Component_" + std::to_string(i));
		Component* component = GetComponent((Component::ComponentType)data.GetInt("Type"));
		if (component != nullptr) {
			component->Load(data);
		}
		else {
			AddComponent((Component::ComponentType)data.GetInt("Type"));
			GetComponent((Component::ComponentType)data.GetInt("Type"))->Load(data);
		}
		data.LeaveSection();
	}
	data.LeaveSection();

	UID parent_id = data.GetUInt("ParentID");
	if (parent_id != 0) {
		SetParentByID(parent_id);
	}

	is_root = data.GetBool("IsRoot");

	if (!is_prefab) {
		//Store gameObject name to know the existing gameObjects when loading scene
		int gameObjectCount = 1;
		bool inParenthesis = false;
		std::string str;
		std::string tempName = name;
		for (int i = 0; i < name.size(); i++) {
			if (name[i] == ')') {
				inParenthesis = false;
				if (name[i + 1] == '\0') {
					break;
				}
				else {
					str.clear();
				}
			}
			if (inParenthesis) {
				str.push_back(name[i]);
			}
			if (name[i] == '(') {
				inParenthesis = true;
			}
		}
		if (atoi(str.c_str()) != 0) {
			name.erase(name.end() - (str.length() + 2), name.end());
			gameObjectCount = stoi(str);
		}

		std::map<std::string, int>::iterator it = App->scene->scene_gameobjects_name_counter.find(name);
		if (it != App->scene->scene_gameobjects_name_counter.end()) {
			if (App->scene->scene_gameobjects_name_counter[name] < gameObjectCount) {
				App->scene->scene_gameobjects_name_counter[name] = gameObjectCount;
			}
		}
		else {
			App->scene->scene_gameobjects_name_counter[name] = 1;
		}
		name = tempName;
	}

}

void GameObject::DeleteFromResourcesDestructor()
{
	for (std::list<Component*>::iterator it = components_list.begin(); it != components_list.end(); ++it) {
		RELEASE(*it);
	}
}
