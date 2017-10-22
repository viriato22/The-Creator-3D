#include "Resource.h"
#include "Application.h"

Resource::Resource()
{
	name = "";
	path = "";
	uuid = App->RandomNumber().Int();
}

Resource::~Resource()
{
}

std::string Resource::GetName() const
{
	return name;
}

std::string Resource::GetPath() const
{
	return path;
}

UID Resource::GetUID() const
{
	return uuid;
}

void Resource::SetName(std::string name)
{
	this->name = name;
}

void Resource::SetPath(std::string path)
{
	this->path = path;
}

void Resource::SetUID(UID uid)
{
	uuid = uid;
}

Resource::ResourceType Resource::GetType() const
{
	return type;
}

void Resource::SetType(ResourceType type)
{
	this->type = type;
}