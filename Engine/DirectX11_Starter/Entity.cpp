#include "Entity.h"
#include "ComponentManager.h"
#include "Renderable.h"

using namespace std;

static EntityID nextID = 0;

//TODO: delete and use entity manager
static vector<Entity*> entities;

// alive entity list
static list<Entity*> allEntities;

Entity::Entity() 
	: name("Entity"), id(nextID++)
{
	AddComponent<Transform>();
	entities.push_back(this);
	allEntities.push_back(this);
}

Entity::Entity(string name)
	: name(name), id(nextID++)
{
	AddComponent<Transform>();
	entities.push_back(this);
	allEntities.push_back(this);
}

Entity::Entity(std::string name, Material * mat, Mesh * mesh)
	: name(name), id(nextID++)
{
	AddComponent<Transform>();
	AddComponent<Renderable>(mesh, mat);
	entities.push_back(this);
	allEntities.push_back(this);
}

Entity::~Entity()
{
	ComponentManager::current->RemoveAllComponents(GetID());
	allEntities.remove(this);
}

Entity * Entity::GetEntity(EntityID eid)
{
	if (eid >= nextID)
		throw "ID is invalid, too big!";

	return entities[eid];
}

const list<Entity*> Entity::GetAllEntities()
{
	return allEntities;
}

bool Entity::RemoveComponent(TypeId typeID)
{
	return ComponentManager::current->RemoveComponent(GetID(), typeID);
}

string Entity::GetName()
{
	return name;
}

std::vector<Component*> Entity::GetAllComponents()
{
	return ComponentManager::current->GetAllComponents(GetID());
}

void Entity::ChangeName(std::string name)
{
	this->name = name;
}

void Entity::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
{
	//name
	writer.StartObject();
	writer.String("name");
	writer.String(this->name.c_str());
	writer.String("mesh");
	writer.String(mesh.c_str());
	writer.String("material");
	writer.String(material.c_str());
	writer.String("components");
	//components
	writer.StartArray();
	for (auto c : GetAllComponents()) {
		c->Serialize(writer);
	}
	writer.EndArray();
	writer.EndObject();
}
