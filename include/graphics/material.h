#pragma once

#include <vector>
#include <memory>

#include "texture.h"
#include "shader.h"

namespace engine::graphics {
	class Material {
		private:
		std::vector<std::unique_ptr<Texture>> textures;
		static int maxTextures;
		int matID;
		Shader shader;

		public:
		Material(Shader &sh);
		~Material() = default;

		int AddTexture(const std::string &img, bool linear = true, bool mipmap = true, Texture::TextureType textureType = Texture::TextureType::COLOR);
		void RemoveTexture(int id);

		const Texture &GetTexture(int id) const;

		void Use();
	};
}