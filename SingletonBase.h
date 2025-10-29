#pragma once
template<typename SingletonName>
class SingletonBase
{
protected:
	SingletonBase() = default;

public:
	SingletonName& getInstance();

	SingletonBase(const SingletonBase&) = delete;
	SingletonBase& operator=(const SingletonBase&) = delete;
};

template<typename SingletonName>
inline SingletonName& SingletonBase<SingletonName>::getInstance()
{
	static SingletonName& instance;
	return instance;
}
