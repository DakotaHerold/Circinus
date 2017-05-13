#pragma once
#include <map>

class Object
{
public:
	Object();
	virtual ~Object();
	int GetID() const;	
	template <class T>
	static T* GetObjectWithID(int id);
private:
	int id;
	static int nextID;
	static std::map<int, Object*> allObjects;
};

template <class T>
T* Object::GetObjectWithID(int id) {

	static_assert(std::is_base_of<Object, T>(), "T is not a object");

	if (id >= nextID)
		throw "ID is invalid, too big!";

	return dynamic_cast<T*>(allObjects[id]);
}
