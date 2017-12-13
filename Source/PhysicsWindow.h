#pragma once

#include "Window.h"

class PhysicsWindow :
	public Window
{
public:
	PhysicsWindow();
	virtual ~PhysicsWindow();

	void DrawWindow();

private:
	//void SelectObjects(GameObject* object1, GameObject* object2); //For some reason it doesn't work
};