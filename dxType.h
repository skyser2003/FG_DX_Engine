#pragma once

namespace FG
{
	template <int D>
	struct VECTOR
	{
	public:
		float& operator[](int index)
		{
			return vector[index];
		}

		float operator[](int index) const
		{
			return vector[index];
		}
	private:
		float vector[D];
	};

	template <>
	struct VECTOR<2>
	{
	public:
		int x, y;
	};

	template <>
	struct VECTOR<3>
	{
	public:
		int x, y, z;
	};

	template <>
	struct VECTOR<4>
	{
	public:
		int x, y, z, w;
	};
}