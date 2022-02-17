#pragma once

#include <GL/glew.h>

#include <string>

#include "resource/resource.hpp"

namespace Oasis
{
	class Texture : public Resource {

	public:
		static Resource * Load(const std::string& path);

		Texture(int width, int height);
		Texture(const std::string& path);
		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		void SetData(void* data, uint32_t size, GLenum format = GL_RGBA);

		inline int getWidth() const { return width; }
		inline int getHeight() const { return height; }
	private:
		GLuint textureID;
		std::string filePath;
		unsigned char * localBuffer;
		int width, height, bitsPerPixel;
	};
}