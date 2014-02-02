////////////////////////////////////////////////////////////////////////////////
// Filename: fontclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_


//////////////
// INCLUDES //
//////////////

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
namespace FG
{
	class TextureClass;

	////////////////////////////////////////////////////////////////////////////////
	// Class name: FontClass
	////////////////////////////////////////////////////////////////////////////////
	class FontClass
	{
	private:
		struct FontType
		{
			float left, right;
			int size;
		};

		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 texture;
			D3DXVECTOR4 color;
		};

	public:
		FontClass();
		FontClass(const FontClass&);
		~FontClass();

		bool Initialize(ID3D11Device*, const char*);
		void Shutdown();

		void BuildVertexArray(void*, char*, float, float, float, float, float, float, float);

	private:
		bool LoadFontData(const char*);
		void ReleaseFontData();
		void ReleaseTexture();

	private:
		FontType* m_Font;
		TextureClass* m_Texture;
	};
}
#endif