#pragma once

#include <utility>
#include <stdint.h>
#include <stdlib.h> 
#include <new>
#include <assert.h>

class ComponentPoolBase
{
public:
	ComponentPoolBase() {};
	virtual ~ComponentPoolBase() {};
};

//typedef uint16_t pSize;
typedef int typePoolIndex;

template <typename T>
struct ResultComponents {
	T* components;
	int size;
};

template <typename T>
class ComponentPool : public ComponentPoolBase
{
public:
	ComponentPool(int initialSize, int resizeAmount, bool isResizeAllowed = true);
	~ComponentPool();

	int GetCount();
	int GetValidCount();

	template <class... Args>
	T* AddComponent(Args&& ...args);
	T* GetComponent(typePoolIndex index);
	ResultComponents<T> GetAllComponents();

	void ReturnComponent(T* component);

private:
	int				m_length;
	int				m_resizeAmount;
	bool			m_isResizeAllowed;

	T*				m_components;
	int				m_count;
};

template<typename T>
ComponentPool<T>::ComponentPool(int initialSize, int resizeAmount, bool isResizeAllowed)
	:m_length(initialSize), m_resizeAmount(resizeAmount), m_isResizeAllowed(isResizeAllowed)
{
	m_components = reinterpret_cast<T *>(malloc(sizeof(T) * initialSize));

	if (m_components == nullptr) {
		throw "alloc error!";
	}

	if(m_isResizeAllowed)
		assert(resizeAmount != 0);

	m_count = 0;

	//for (int i = 0; i < initialSize; i++) {
	//	cout << m_components + i << endl;
	//}
}

template<typename T>
ComponentPool<T>::~ComponentPool()
{
	assert(m_components != nullptr);

	for (int i = 0; i < m_count; i++) {
		// http://stackoverflow.com/questions/2995099/malloc-and-constructors
		// http://en.cppreference.com/w/cpp/language/new
		reinterpret_cast<T *>(m_components + i)->~T();

		//delete (m_components + i);
	}

	free(m_components);
}

template<typename T>
inline int ComponentPool<T>::GetCount()
{
	return m_count;
}

template<typename T>
inline int ComponentPool<T>::GetValidCount()
{
	return m_length - m_count;
}

template<typename T>
template<typename ...Args>
inline T * ComponentPool<T>::AddComponent(Args && ...args)
{
	if (m_count == m_length)
	{
		// if we can't resize, then we can not give the user back any instance.
		if (!m_isResizeAllowed)
		{
			throw "limit exceeded length, and the pool was set to not resize.";
		}

		// Create a new array with some more slots and copy over the existing m_components.
		//T* newComponents = new T[m_length + m_resizeAmount];

		m_length += m_resizeAmount;

		m_components = reinterpret_cast<T *>(realloc(m_components, sizeof(T) * m_length));

		if (m_components == nullptr) {
			throw "realloc error!";
		}
	}

	// TODO: Replace T's variable when T is already existed at the position 
	T* result = new (m_components + m_count) T(std::forward<Args>(args)...);
	// TODO: Get from global allocator
	result->poolIndex = new int(m_count++);

	return result;
}

template<typename T>
inline T * ComponentPool<T>::GetComponent(typePoolIndex index)
{
	assert(index < m_length);
	return &m_components[index];
}

template<typename T>
inline ResultComponents<T> ComponentPool<T>::GetAllComponents()
{
	return {m_components, m_count};
}

template<typename T>
inline void ComponentPool<T>::ReturnComponent(T * component)
{
	assert(component >= m_components && component < m_components + m_length);

	--count;

	*(m_components[count].poolIndex) = *(component->poolIndex);

	m_components[component->poolIndex] = m_components[count];

	// TODO: Reset component's variable to reuse object
	//m_componets[count].poolIndex = -1;
}
