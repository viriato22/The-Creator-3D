#include "Component.h"

Component::Component()
{
	type = Unknown;
	active = true;
	gameObject = nullptr;
	name = "Unknown";
}

Component::~Component()
{
}

void Component::OnEnable()
{
}

void Component::OnDisable()
{
}

void Component::SetActive(bool active)
{
	this->active = active;
	if (this->active) {
		OnEnable();
	}
	else {
		OnDisable();
	}
}

bool Component::IsActive() const
{
	return active;
}

void Component::SetGameObject(GameObject * attached_gameobject)
{
	gameObject = attached_gameobject;
}

GameObject * Component::GetGameObject() const
{
	return gameObject;
}

void Component::SetType(ComponentType type)
{
	this->type = type;
}

Component::ComponentType Component::GetType() const
{
	return type;
}

void Component::SetName(std::string name)
{
	this->name = name;
}

std::string Component::GetName() const
{
	return name;
}

void Component::Save(Data & data) const
{
}

void Component::Load(Data & data)
{
}