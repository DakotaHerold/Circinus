#pragma once
#include "ClassTypeId.h"
#include <rapidjson\prettywriter.h>
#include <rapidjson\document.h>
#include <utility>
#include "ObjectPool.h"
#include "Entity.h"

class Component : public Poolable
{
public:
	Component();
	~Component();

	virtual void				Update();

	virtual void				SetEntity(EntityID eid);
	Entity*						GetEntity();
	int							GetEntityID();

	virtual void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		//
		writer.StartObject();
		writer.String("name");
		writer.String("Undecided");
		writer.EndObject();
	}

	virtual void Load(rapidjson::Value v) {
		//
		
	}

private:
	EntityID 					gameEntityID;
};

template <typename T>
TypeId ComponentTypeId()
{
	return ClassTypeId<Component>::GetTypeId<T>();
}

static char * ComponentTypeName(TypeId id)
{
	return ClassTypeId<Component>::GetTypeName(id);
}


