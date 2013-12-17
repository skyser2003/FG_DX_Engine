#include "dxstdafx.h"
#include "TextureManager.h"

#include "textureclass.h"

TextureManager::TextureManager() : mDevice(nullptr), nextTextureID(0)
{

}

std::shared_ptr<TextureClass> TextureManager::GetTexture(const std::string& fileName)
{
	if (mDevice == nullptr) {
		return nullptr;
	}

	std::shared_ptr<TextureClass> texture(new TextureClass);
	bool ret = texture->Initialize(mDevice, fileName.c_str());

	if (ret == false) {
		return nullptr;
	}

	auto it = mTextures.find(fileName);
	if (it != mTextures.end()) {
		return it->second;
	}

	mTextures.insert(std::make_pair(fileName, texture));
	return texture;
}