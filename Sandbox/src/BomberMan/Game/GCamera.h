#pragma once

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

using namespace Hurikan;

class GCamera
{
public:
	void Create(Entity& e);
private:
	Entity m_Handle;
};