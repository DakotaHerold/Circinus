#pragma once
#include <vector>
using namespace std;
class Object
{
public:
	Object();
	~Object();
	int GetID();

	template <typename T>
	static T* GetObjectWithID(int id);
private:
	int id;
	static int nextID;
	static vector<pair<int, Object*>> allObjects;
};
