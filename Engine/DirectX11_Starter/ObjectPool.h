#pragma once

#include <stdlib.h>
#include <utility>
#include <new>
#include <assert.h>

typedef int ObjectPoolIndex;

static const int ObjectPoolInitialLength = 100;
static const int ObjectPoolResizeAmount = 50;

class Poolable {

	template <typename U>
	friend class ObjectPool;

public:
	Poolable() {};

	virtual ~Poolable() {
		if (poolIndex)
			delete poolIndex;
	};

	ObjectPoolIndex*	getPoolIndex() { return poolIndex; }

private:
	ObjectPoolIndex*	poolIndex = nullptr;
};

class ObjectPoolBase
{
public:
	ObjectPoolBase() {};
	virtual ~ObjectPoolBase() {};

	virtual void Return(ObjectPoolIndex index) = 0;
};

template <typename T>
struct ResultComponents {
	T* components;
	int size;
};

template <typename T>
class ObjectPool : public ObjectPoolBase
{
public:
	ObjectPool(int length = ObjectPoolInitialLength, int resizeAmount = ObjectPoolResizeAmount, bool isResizeAllowed = ObjectPoolResizeAmount == 0 ? false : true);
	~ObjectPool();

	int GetCount();
	int GetValidCount();

	template <class... Args>
	T* Add(Args&& ...args);
	T* Get(ObjectPoolIndex index);
	void Return(ObjectPoolIndex index) override;

	ResultComponents<T> GetAllComponents();

private:
	int		m_size;					// object size

	int		m_length;
	int		m_resizeAmount;
	bool	m_isResizeAllowed;

	T*		m_objects;
	int		m_count;
};

template<typename T>
ObjectPool<T>::ObjectPool(int length, int resizeAmount, bool isResizeAllowed)
	:m_size(sizeof(T)),	m_length(length), m_resizeAmount(resizeAmount), m_isResizeAllowed(isResizeAllowed)
{
	static_assert(std::is_base_of<Poolable, T>(), "T is not a poolable object, cannot make a ObjectPool for it.");

	if (m_isResizeAllowed)
		assert(resizeAmount != 0);
	
	m_objects = reinterpret_cast<T *>(malloc(m_size * length));

	if (m_objects == nullptr) {
		throw "alloc error!";
	}

	m_count = 0;
}

template<typename T>
ObjectPool<T>::~ObjectPool()
{
	assert(m_objects != nullptr);

	for (int i = 0; i < m_count; i++) {
		// http://stackoverflow.com/questions/2995099/malloc-and-constructors
		// http://en.cppreference.com/w/cpp/language/new
		//reinterpret_cast<T *>(m_objects + i)->~T();
		m_objects[i].~T();

		//delete (m_objects + i);
	}

	free(m_objects);
}

template<typename T>
inline int ObjectPool<T>::GetCount()
{
	return m_count;
}

template<typename T>
inline int ObjectPool<T>::GetValidCount()
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

		// Create a new array with some more slots and copy over the existing m_objects.
		//T* newComponents = new T[m_length + m_resizeAmount];

		m_length += m_resizeAmount;

		m_objects = reinterpret_cast<T *>(realloc(m_objects, sizeof(T) * m_length));

		if (m_objects == nullptr) {
			throw "realloc error!";
		}
	}

	T* result = new (m_objects + m_count) T(std::forward<Args>(args)...);
	// TODO: Get from global allocator
	result->poolIndex = new int(m_count++);

	return result;
}

template<typename T>
inline T * ObjectPool<T>::Get(ObjectPoolIndex index)
{
	assert(index < m_length);
	return m_objects + index;
}

template<typename T>
inline void ObjectPool<T>::Return(ObjectPoolIndex index)
{
	assert(index < m_count);

	--m_count;

	memcpy(m_objects + index, m_objects + m_count, m_size);
	*(m_objects[index].poolIndex) = index;

	m_objects[m_count].~T();
}

template<typename T>
inline ResultComponents<T> ObjectPool<T>::GetAllComponents()
{
	return {m_objects, m_count};
}
