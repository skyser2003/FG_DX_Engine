#include "dxstdafx.h"
#include "dxType.h"

namespace FG
{
	VECTOR2::operator float*()
	{
		return static_cast<float*>(&x);
	}
	VECTOR2::operator const float*() const
	{
		return static_cast<const float*>(&x);
	}

	VECTOR3::operator float*()
	{
		return static_cast<float*>(&x);
	}
	VECTOR3::operator const float*() const
	{
		return static_cast<const float*>(&x);
	}

	VECTOR4::operator float*()
	{
		return static_cast<float*>(&x);
	}
	VECTOR4::operator const float*() const
	{
		return static_cast<const float*>(&x);
	}


}