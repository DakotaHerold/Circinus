#pragma once

#include <utility>
#include <stdint.h>

//typedef uint16_t pSize;

template <typename T>
class ComponentPool
{
public:
	ComponentPool(int initialSize, int resizeAmount, bool isResizeAllowed = true);
	~ComponentPool();

	int GetInvalidCount();
	int GetValidCount();

	template <class... Args>
	T* AddComponent(Args&& ...args);

	//void ReturnComponent(T component);

private:
	int		m_resizeAmount;
	bool	m_isResizeAllowed;
	int		m_invalidCount;

	T*		m_components;
	int		m_length;
};

template<typename T>
ComponentPool<T>::ComponentPool(int initialSize, int resizePool, bool isResizeAllowed)
{
	m_components = new T[initialSize];

	m_length = initialSize;
	m_isResizeAllowed = isResizeAllowed;

	m_invalidCount = m_length;
}

template<typename T>
ComponentPool<T>::~ComponentPool()
{
	assert(m_components != nullptr);
	delete[] m_components;
}

template<typename T>
int ComponentPool<T>::GetInvalidCount()
{
	return m_invalidCount;
}

template<typename T>
int ComponentPool<T>::GetValidCount()
{
	return m_length - this->GetInvalidCount();
}

template<typename T>
template<typename ...Args>
inline T * ComponentPool<T>::AddComponent(Args && ...args)
{
	if (this->GetInvalidCount() == 0)
	{
		// if we can't resize, then we can not give the user back any instance.
		if (!m_isResizeAllowed)
		{
			throw "limit exceeded length, and the pool was set to not resize.";
		}

		// Create a new array with some more slots and copy over the existing m_components.
		T* newComponents = new T[m_length + m_resizeAmount];

		for (int index = m_length - 1; index >= 0; --index)
		{
			/*if (index >= this->GetInvalidCount())
			{
			this.m_components[index].PoolId = index + this.ResizeAmount;
			}*/

			newComponents[index + m_resizeAmount] = m_components[index];
		}

		m_components = newComponents;

		// move the invalid count based on our resize amount
		m_invalidCount += m_resizeAmount;
	}

	// decrement the counter
	--m_invalidCount;

	T test = m_components[m_invalidCount];

	static_assert(&test != nullptr, "??");

	// TODO: Replace variable
	m_components[m_invalidCount] = T(std::forward<Args>(args)...);

	Component result = static_cast<Component>(m_components[m_invalidCount]);

	result.poolIndex = m_invalidCount;

	return &m_components[m_invalidCount];

	//// get the next component in the list
	//T result = m_components[m_invalidCount];

	//// if the component is null, we need to allocate a new instance
	//if (result == null)
	//{
	//	result = this.allocate(this.innerType);

	//	if (result == null)
	//	{
	//		throw new InvalidOperationException("The pool's allocate method returned a null object reference.");
	//	}

	//	this.m_components[this.InvalidCount] = result;
	//}

	//result.PoolId = this.InvalidCount;

	// Initialize the object if a delegate was provided.
	//result.Initialize();

	//return result;

	//return nullptr;
}