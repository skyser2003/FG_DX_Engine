#pragma once

#include "dxType.h"

namespace FG
{
	struct RenderInfo
	{
		int noVertices;
		std::vector<VECTOR3> position;
		VECTOR4 color;
		std::vector<VECTOR2> texPosition;
		std::vector<VECTOR3> normal;
	};
}