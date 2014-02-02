////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once
///////////////////////
// MY CLASS INCLUDES //
///////////////////////

namespace FG
{
	class GraphicsClass;

	////////////////////////////////////////////////////////////////////////////////
	// Class name: ModelClass
	////////////////////////////////////////////////////////////////////////////////
	class ModelClass
	{
	public:
		ModelClass();
		ModelClass(const ModelClass&);
		~ModelClass();

		void SetR(float r);
		void SetG(float g);
		void SetB(float b);
		void SetA(float a);
		void SetRGBA(float r, float g, float b, float a);
		void SetRGBA(D3DXVECTOR4 rgba);
		void SetVertex(ID3D11Device* device, int noVertices, void* buffer, int bufferSize);
		void SetRotation(float x, float y, float z);

		void Render(ID3D11DeviceContext*);
		void Shutdown();

		int GetIndexCount() const;

		ID3D11Buffer* GetVertexBuffer() const;
		ID3D11Buffer* GetIndexBuffer() const;
	private:
		float r, g, b, a;
		float rotationX, rotationY, rotationZ;
		D3DXMATRIX rotation;

		void ShutdownBuffers();
		void RenderBuffers(ID3D11DeviceContext*);
		void Rotate(float centerX, float centerY, float halfWidth, float halfHeight, float rotationAngle);

		//std::unique_ptr<ID3D11Buffer> m_vertexBuffer,m_indexBuffer;
		ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
		int m_vertexCount, m_indexCount;
	};
}