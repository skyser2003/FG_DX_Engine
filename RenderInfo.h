#pragma once

struct RenderInfo
{
	int noVertices;
	std::vector<D3DXVECTOR3> position;
	D3DXVECTOR4 color;
	std::vector<D3DXVECTOR2> texPosition;
	std::vector<D3DXVECTOR3> normal;

	ID3D11ShaderResourceView* texture;
};