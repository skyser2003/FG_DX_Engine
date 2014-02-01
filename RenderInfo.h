#pragma once


namespace FG
{
	class TextureClass;

	struct RenderInfo
	{
		void* buffer = nullptr;
		int noVertices = 0;
		int bufferSize = 0;

		TextureClass* texture = nullptr;
	};
}