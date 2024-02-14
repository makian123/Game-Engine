#pragma once

#include <string>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_opengl.h>

namespace engine::graphics {
	class Texture {
		public:
		enum class TextureType : int {
			COLOR,
			NORMAL,
			ROUGHNESS
		};

		private:
		GLuint id = 0;
		int w = 0, h = 0, channelCount = 0;
		bool isNormal;
		TextureType type;
		public:
		Texture() = default;
		Texture(const std::string &img, bool linear = true, bool mipmap = true, TextureType textureType = TextureType::COLOR);
		~Texture();

		void Use();
		GLuint GetID() const { return id; }
	};
}