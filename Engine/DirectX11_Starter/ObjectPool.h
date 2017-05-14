#pragma once

#include <stdlib.h>
#include <utility>
#include <new>
#include <assert.h>
#include <map>
#include <vector>
#include <algorithm>

typedef unsigned int ObjectPoolIndex;

static const ObjectPoolIndex ObjectPoolInitialLength = 100;
static const ObjectPoolIndex ObjectPoolResizeAmount = 50;

class Poolable {

	template <typename U>
	friend class ObjectPool;

public:
	Poolable() {};
	virtual ~Poolable() {};

	ObjectPoolIndex* getPoolIndex() { return poolIndex; }

private:
	ObjectPoolIndex* poolIndex = nullptr;
};

class ObjectPoolBase
{
public:
	ObjectPoolBase() {};
	virtual ~ObjectPoolBase() {};

	virtual void* Get(ObjectPoolIndex index) = 0;
	virtual void Return(ObjectPoolIndex index) = 0;
};

template <typename T>
class ObjectPool : public ObjectPoolBase
{
public:
	ObjectPool(int length = ObjectPoolInitialLength, int resizeAmount = ObjectPoolResizeAmount, bool isResizeAllowed = ObjectPoolResizeAmount == 0 ? false : true);
	~ObjectPool();

	ObjectPoolIndex GetCount();
	ObjectPoolIndex GetValidCount();

	template <class... Args>
	T* Add(Args&& ...args);
	void* Get(ObjectPoolIndex index) override;
	void Return(ObjectPoolIndex index) override;

	std::vector<T*> GetAllComponents();

private:
	void AllocMemory(ObjectPoolIndex size);
	T* GetAddress(ObjectPoolIndex index);

	ObjectPoolIndex		m_size;					// object size

	ObjectPoolIndex		m_length;
	ObjectPoolIndex		m_resizeAmount;
	bool				m_isResizeAllowed;

	std::map<ObjectPoolIndex, T*>	m_objectsMap;
	std::vector<T*> m_allObjects;

	std::vector<T*> m_availableObjects;

	ObjectPoolIndex		m_count;
};

template<typename T>
ObjectPool<T>::ObjectPool(int length, int resizeAmount, bool isResizeAllowed)
	:m_size(sizeof(T)),	m_length(0), m_resizeAmount(resizeAmount), m_isResizeAllowed(isResizeAllowed)
{
	static_assert(std::is_base_of<Poolable, T>(), "T is not a poolable object, cannot make a ObjectPool for it.");

	if (m_isResizeAllowed) {
		if(resizeAmount == 0)
			throw "Please provide a valid resize amount.";
	}
	
	AllocMemory(length);
	m_count = 0;
}

template<typename T>
ObjectPool<T>::~ObjectPool()
{
	for (auto i : m_allObjects) {
		i->~T();
	}

	for (auto rit = m_objectsMap.rbegin(); rit != m_objectsMap.rend(); ++rit) {
		free(rit->second);
	}

	//for (ObjectPoolIndex i = 0; i < m_count; i++) {
	//	// http://stackoverflow.com/questions/2995099/malloc-and-constructors
	//	// http://en.cppreference.com/w/cpp/language/new
	//	m_objects[i].~T();

	//	//delete (m_objects + i);
	//}
	
	//free(m_objects);
}

template<typename T>
inline ObjectPoolIndex ObjectPool<T>::GetCount()
{
	return m_count;
}

template<typename T>
inline ObjectPoolIndex ObjectPool<T>::GetValidCount()
{
	return m_length - m_count;
}

template<typename T>
template<typename ...Args>
inline T* ObjectPool<T>::Add(Args && ...args)
{
	if (m_count == m_length)
	{
		// if we can't resize, then we can not give the user back any instance.
		if (!m_isResizeAllowed)
		{
			throw "limit exceeded length, and the pool was set to not resize.";
		}

		AllocMemory(m_resizeAmount);
	}

	T* result = new (GetAddress(m_count)) T(std::forward<Args>(args)...);
	// TODO: Get from global allocator
	result->poolIndex = new ObjectPoolIndex(m_count++);

	m_allObjects.push_back(result);
	m_availableObjects.push_back(result);

	return result;
}

template<typename T>
inline void* ObjectPool<T>::Get(ObjectPoolIndex index)
{
	if (index >= m_count) {
		throw "index out of count!";
	}
	//return GetAddress(index);

	return reinterpret_cast<void*>(m_allObjects[index]);
}

template<typename T>
inline void ObjectPool<T>::Return(ObjectPoolIndex index)
{
	if (index >= m_count) {
		throw "index out of count!";
	}

	// FIXME: Does not move memory to prevent possible pointer issue

	//--m_count;

	T* del = GetAddress(index);

	delete del->poolIndex;
	del->~T();

	m_availableObjects.erase(std::remove(m_availableObjects.begin(), m_availableObjects.end(), del), m_availableObjects.end());

	//if (index != m_count) {
	//	memcpy(del, GetAddress(m_count), m_size);
	//	*(del->poolIndex) = index;

	//	// FIXME: Should I erase memory??
	//	memset(GetAddress(m_count), 0, m_size);
	//}

	//assert(m_allObjects.back() == GetAddress(m_count));
	//m_allObjects.pop_back();
}

template<typename T>
inline std::vector<T*> ObjectPool<T>::GetAllComponents()
{
	//return {m_objects, m_count};
	//return m_allObjects;
	return m_availableObjects;
}

template<typename T>
inline void ObjectPool<T>::AllocMemory(ObjectPoolIndex length)
{
	T *m_objects = reinterpret_cast<T *>(malloc(m_size * length));

	if (m_objects == nullptr) {
		throw "alloc error!";
	}

	m_objectsMap[m_length] = m_objects;

	m_length += length;
}

// FIXME: Duplicated with Get()
template<typename T>
inline T * ObjectPool<T>::GetAddress(ObjectPoolIndex index)
{
	if (index < m_count)
		return m_allObjects[index];

	// TODO: change by next index
	for (auto rit = m_objectsMap.rbegin(); rit != m_objectsMap.rend(); ++rit) {
		if (rit->first <= index) {
			return rit->second + (index - rit->first);
		}
	}

	return nullptr;
}
