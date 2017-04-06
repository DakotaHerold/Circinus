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


int Object::GetID() const
{
	return id;
}


int Object::nextID = 0;
vector<pair<int, Object*>> Object::allObjects;
