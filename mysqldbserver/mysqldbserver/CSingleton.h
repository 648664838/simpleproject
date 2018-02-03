#pragma once

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else  /* __cplusplus */
#define NULL    ((void *)0)
#endif  /* __cplusplus */
#endif  /* NULL */

template< typename T >
class CMySingleton
{
protected:
	static T* spSingleton;
	CMySingleton()
	{
		spSingleton = static_cast<T*>(this);
	}
public:
	static T* GetSingletonPtr()
	{
		if (spSingleton == NULL)
		{
			return new T;
		}
		return spSingleton;
	}

	static void Destory()
	{
		if (spSingleton != NULL)
		{
			delete spSingleton;
		}
		spSingleton = NULL;
	}
};
template< typename T >
T* CMySingleton<T>::spSingleton = NULL;
