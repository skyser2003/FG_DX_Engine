#include "dxstdafx.h"

#include "lightclass.h"

LightClass::LightClass()
{

}
LightClass::LightClass(const LightClass& rhs)
{

}
LightClass::~LightClass()
{

}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	diffuseColor = D3DXVECTOR4(r, g, b, a);
}
void LightClass::SetDirection(float x, float y, float z)
{
	direction = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 LightClass::GetDiffuseColor() const
{
	return diffuseColor;
}
D3DXVECTOR3 LightClass::GetDirection() const
{
	return direction;
}