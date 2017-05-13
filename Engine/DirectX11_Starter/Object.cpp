#include "Object.h"

using namespace std;

Object::Object()
{
	id = nextID;
	nextID++;
	allObjects[id] = this;
}

Object::~Object()
{
	allObjects[id] = nullptr;
}

int Object::GetID() const
{
	return id;
}

int Object::nextID = 0;
map<int, Object*> Object::allObjects;
