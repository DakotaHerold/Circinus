#pragma once
#include "ClassTypeId.h"
#include "Object.h"
#include <rapidjson\prettywriter.h>
#include <utility>
#include "ObjectPool.h"

class Entity;

class Component : public Object, public Poolable
{
public:
	Component();
	~Component();

	virtual void				Update();

	virtual void				SetEntity(int id);
	Entity*						GetEntity();
	int							GetEntityID();

	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
		//
		writer.StartObject();
		writer.String("hasValue");
		writer.Bool(false);
		writer.EndObject();
	}

private:
	int							gameEntityID;
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


