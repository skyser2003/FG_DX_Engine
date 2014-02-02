#pragma once

namespace FG
{
	class TextureClass;

	class TextureManager
	{
	public:
		TextureManager();

		std::shared_ptr<TextureClass> CreateTexture(const std::string& fileName);

		void SetDevice(ID3D11Device* device);
	private:
		std::map<std::string, std::shared_ptr<TextureClass>> mTextures;

		ID3D11Device* mDevice;
		int nextTextureID;
	};
}