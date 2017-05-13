#pragma once

#include <stdlib.h>
#include <utility>
#include <new>
#include <assert.h>
#include <map>
#include <vector>

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

	virtual void Return(ObjectPoolIndex index) = 0;
};

//template <typename T>
//struct ResultComponents {
//	T* components;
//	unsigned int size;
//};

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
	T* Get(ObjectPoolIndex index);
	void Return(ObjectPoolIndex index) override;

	std::vector<T *> GetAllComponents();

private:
	void AllocMemory(ObjectPoolIndex size);
	T* GetAddress(ObjectPoolIndex index);

	ObjectPoolIndex		m_size;					// object size

	ObjectPoolIndex		m_length;
	ObjectPoolIndex		m_resizeAmount;
	bool				m_isResizeAllowed;

	//T*					m_objects;
	std::map<ObjectPoolIndex, T*>	m_objectsMap;
	std::vector<T *> m_allObjects;

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
inline T * ObjectPool<T>::Add(Args && ...args)
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


	T* addre = GetAddress(m_count);

	T* result = new (GetAddress(m_count)) T(std::forward<Args>(args)...);
	// TODO: Get from global allocator
	result->poolIndex = new ObjectPoolIndex(m_count++);

	m_allObjects.push_back(result);

	return result;
}

template<typename T>
inline T * ObjectPool<T>::Get(ObjectPoolIndex index)
{
	if (index >= m_count) {
		throw "index out of count!";
	}
	return GetAddress(index);
}

template<typename T>
inline void ObjectPool<T>::Return(ObjectPoolIndex index)
{
	if (index >= m_count) {
		throw "index out of count!";
	}

	--m_count;

	T* del = GetAddress(index);

	delete del->poolIndex;
	del->~T();

	if (index != m_count) {
		memcpy(del, GetAddress(m_count), m_size);
		*(del->poolIndex) = index;

		// FIXME: Should I erase memory??
		memset(GetAddress(m_count), 0, m_size);
	}

	assert(m_allObjects.back() == GetAddress(m_count));
	m_allObjects.pop_back();
}

template<typename T>
inline std::vector<T *> ObjectPool<T>::GetAllComponents()
{
	//return {m_objects, m_count};
	return m_allObjects;
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

template<typename T>
inline T * ObjectPool<T>::GetAddress(ObjectPoolIndex index)
{
	for (auto rit = m_objectsMap.rbegin(); rit != m_objectsMap.rend(); ++rit) {
		if (rit->first <= index) {
			return rit->second + (index - rit->first);
		}
	}
}
