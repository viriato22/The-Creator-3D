#pragma once
#include "Component.h"

class ComponentPhysics :
	public Component
{
public:
	ComponentPhysics(GameObject* attached_gameobject);
	virtual ~ComponentPhysics();

	void Save(Data& data) const;
	void Load(Data& data);
};

