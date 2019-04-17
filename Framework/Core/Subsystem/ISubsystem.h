#pragma once
#include "stdafx.h"

class ISubsystem
{
public:
	ISubsystem(class Context* context)
		:context(context)
	{

	}
	virtual ~ISubsystem() = default;
	virtual const bool Initialize() = 0;

protected:
	Context* context;
};