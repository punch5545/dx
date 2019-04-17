#pragma once
#include "stdafx.h"
#include "./Subsystem/ISubsystem.h"

class Context final	
{
public:
	Context() = default;
	~Context()
	{
		for(int i = static_cast<int>(subsystems.size()-1);i>=0;i--)
			SAFE_DELETE(subsystems[i]);
	}

	auto Initialize() -> const bool
	{
		auto result = true;
		for (auto& subsystem : subsystems)
		{
			if (!subsystem->Initialize())
			{
				result = false;
			}
		}
		return result;
	}

	template <typename T>
	T* RegisterSubSystem();

	template <typename T>
	T* GetSubSystem();
private:
	std::vector<ISubsystem*> subsystems;
};

template<typename T>
inline T * Context::RegisterSubSystem()
{
	static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem.");

	return static_cast<T*>(subsystems.emplace_back(new T(this)));
}
template<typename T>
inline T * Context::GetSubSystem()
{
	//typeid(T);
	static_assert(std::is_base_of<ISubsystem, T>::value, "Provided type does not implement ISubsystem.");
	for (auto subsystem : subsystems) {
		if (typeid(T) == typeid(*subsystem))   // �����ͷ� �����ϸ� upcast�Ǳ� ������ �ڷ����� ����.
			return static_cast<T*>(subsystem); // return���� ISubsystem�̹Ƿ� downcast
	}
	return nullptr;
}
