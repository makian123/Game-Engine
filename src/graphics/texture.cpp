#include "graphics/texture.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

namespace engine::graphics {
	Texture::Texture(const std::string& img, bool linear, bool mipmap, TextureType textureType): type(textureType) {
		uint8_t *data = stbi_load(img.c_str(), &w, &h, &channelCount, 0);

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		if (linear) {
			if (mipmap) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		}
		else {
			if (mipmap) {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		GLint pixelFormat = GL_RGBA;
		switch (channelCount) {
			case 1:
				pixelFormat = GL_RED;
				break;
			case 2:
				pixelFormat = GL_RG;
				break;
			case 3:
				pixelFormat = GL_RGB;
				break;
			case 4:
				pixelFormat = GL_RGBA;
				break;
		}

		if (type == TextureType::COLOR) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		}
		else if(type == TextureType::NORMAL) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		}
		else if (type == TextureType::ROUGHNESS) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
		}
		if (mipmap) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(data);
	}
	Texture::~Texture() {
		glDeleteTextures(1, &id);
	}

	static GLuint currTex = -1;

	void Texture::Use() {
		if (currTex == id) return;

		currTex = id;
		glBindTexture(GL_TEXTURE_2D, id);
	}
}