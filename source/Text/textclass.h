////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Text/fontclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: TextClass
////////////////////////////////////////////////////////////////////////////////
namespace FG
{
	class TextClass
	{
	private:
		struct SentenceType
		{
			ID3D11Buffer *vertexBuffer, *indexBuffer;
			int vertexCount, indexCount, maxLength;
			float red, green, blue;
		};

		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
			D3DXVECTOR4 color;
		};

	public:
		TextClass();
		TextClass(const TextClass&);
		~TextClass();

		static void SetRelativeX(int x);
		static void SetRelativeY(int y);
		static void SetRelativePosition(int x, int y);

		bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX, const char*);
		void Shutdown();
		bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, TextureClass*);
		int GetIndexCount();

		bool InitializeSentence(int, ID3D11Device*);
		bool UpdateSentence(char*, int, int, float, float, float, float, ID3D11DeviceContext*, float);
		void ReleaseSentence();
		bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX, TextureClass*);

	private:
		static int relativeX, relativeY;

		FontClass* m_Font;
		int m_screenWidth, m_screenHeight;
		D3DXMATRIX m_baseViewMatrix;
		SentenceType* m_sentence;
	};
}
#endif