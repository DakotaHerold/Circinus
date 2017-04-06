#pragma once
#include <vector>

using namespace std;
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
	static vector<pair<int, Object*>> allObjects;
};


template <class T>
T* Object::GetObjectWithID(int id) {

	static_assert(std::is_base_of<Object, T>(), "T is not a object");
	for (auto o : allObjects) {
		if (o.first == id) {			
			return dynamic_cast<T*>(o.second);
		}
	}
	return nullptr;
}
