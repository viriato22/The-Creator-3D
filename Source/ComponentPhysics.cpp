#include "ComponentPhysics.h"

ComponentPhysics::ComponentPhysics(GameObject* attached_gameobject) {
	SetActive(true);
	SetName("Physics");
	SetType(ComponentType::Physics);
	SetGameObject(attached_gameobject);
}

ComponentPhysics::~ComponentPhysics(){}

void ComponentPhysics::Save(Data& data) const {
	data.AddInt("Type", GetType());
	data.AddBool("Active", IsActive());
	data.AddUInt("UUID", GetUID());
}


void ComponentPhysics::Load(Data& data){
	SetType((Component::ComponentType)data.GetInt("Type"));
	SetActive(data.GetBool("Active"));
	SetUID(data.GetUInt("UUID"));
}