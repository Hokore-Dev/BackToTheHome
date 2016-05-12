#pragma once
template <class T>

class SingleTon
{
private:
	static T* m_Instance;

public:
	SingleTon()
	{

	};
	virtual ~SingleTon()
	{
		delete m_Instance;
	};
public:
	static T* getInstance()
	{
		if (m_Instance == 0)
		{
			m_Instance = new T();
		}

		return m_Instance;
	};
};
template<class T>T* SingleTon<T>::m_Instance = 0;