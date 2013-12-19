#pragma once

namespace FG
{
	struct VECTOR2
	{
	public:
		operator float*();
		operator const float*() const;

		float x, y;
	};
	struct VECTOR3
	{
	public:
		operator float*();
		operator const float*() const;

		float x, y, z;
	};
	struct VECTOR4
	{
	public:
		operator float*();
		operator const float*() const;

		float x, y, z, w;
	};
}