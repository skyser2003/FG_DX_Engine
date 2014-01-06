#pragma once

class TextureClass;

namespace FG
{
	struct RenderInfo
	{
		int noVertices = 0;
		std::vector<D3DXVECTOR4> position;
		D3DXVECTOR4 color;
		std::vector<D3DXVECTOR2> texPosition;
		std::vector<D3DXVECTOR3> normal;

		TextureClass* texture = nullptr;
	};
}