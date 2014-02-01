#pragma once

namespace FG
{
	class LightClass
	{
	public:
		LightClass();
		LightClass(const LightClass& rhs);
		~LightClass();

		void SetDiffuseColor(float r, float g, float b, float a);
		void SetDirection(float x, float y, float z);

		D3DXVECTOR4 GetDiffuseColor() const;
		D3DXVECTOR3 GetDirection() const;
	private:
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 direction;
	};
}