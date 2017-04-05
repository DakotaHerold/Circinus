#include "Object.h"



Object::Object()
{
	id = nextID;
	nextID++;
	allObjects.push_back(make_pair(id, this));
}


Object::~Object()
{
	for (int i = 0; i < allObjects.size(); i++) {
		if (allObjects[i].first == id) {
			allObjects.erase(allObjects.begin() + i);
			break;
		}
	}
}


int Object::GetID()
{
	return id;
}

template <typename T>
T* Object::GetObjectWithID(int id) {
	for (auto o : allObjects) {
		if (o.first == id) {
			return o.second;
		}
	}
	return nullptr;
}

int Object::nextID = 0;
vector<pair<int, Object*>> Object::allObjects;
