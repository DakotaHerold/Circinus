#include "ComponentPool.h"

//template<class T>
//ComponentPool<T>::ComponentPool(int initialSize, int resizePool)
//{
//	components = new T[initialSize];
//
//	length = sizeof(components) / sizeof(*components);
//	m_InvalidCount = length;
//
//}
//
//template<class T>
//ComponentPool<T>::~ComponentPool()
//{
//}

//template <class T, class... Args>
//T* ComponentPool<T>::AddComponent(Args&&... args)
//{
//	T[] newComponents = new T[length + m_ResizeAmount];
//
//	if (this->GetInvalidCount() == 0)
//	{
//		//// If we can't resize, then we can not give the user back any instance.
//		//if (!this.isResizeAllowed)
//		//{
//		//	throw new Exception("Limit Exceeded " + this.components.Length + ", and the pool was set to not resize.");
//		//}
//
//		// Create a new array with some more slots and copy over the existing components.
//		T[] newComponents = new T[length + m_ResizeAmount];
//
//		for (int index = length - 1; index >= 0; --index)
//		{
//			/*if (index >= this->GetInvalidCount())
//			{
//				this.components[index].PoolId = index + this.ResizeAmount;
//			}*/
//
//			newComponents[index + m_ResizeAmount] = components[index];
//		}
//
//		components = newComponents;
//
//		// move the invalid count based on our resize amount
//		m_InvalidCount += m_ResizeAmount;
//	}
//
//	// decrement the counter
//	--m_InvalidCount;
//
//	// get the next component in the list
//	T result = components[m_InvalidCount];
//
//	// if the component is null, we need to allocate a new instance
//	if (result == null)
//	{
//		result = this.allocate(this.innerType);
//
//		if (result == null)
//		{
//			throw new InvalidOperationException("The pool's allocate method returned a null object reference.");
//		}
//
//		this.components[this.InvalidCount] = result;
//	}
//
//	result.PoolId = this.InvalidCount;
//
//	// Initialize the object if a delegate was provided.
//	result.Initialize();
//
//	//return result;
//
//	return nullptr;
//}

//template<class T>
//void ComponentPool<T>::ReturnComponent(T component)
//{
//}
//
//template<class T>
//int ComponentPool<T>::GetInvalidCount()
//{
//	return m_InvalidCount;
//}
//
//template<class T>
//int ComponentPool<T>::GetValidCount()
//{
//	return length - this->GetInvalidCount();
//}
