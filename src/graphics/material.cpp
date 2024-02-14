#include <graphics/material.h>

#include <SDL/SDL_opengl.h>

namespace engine::graphics {
	int Material::maxTextures = 0;
	static int currMatID = -1;
	static int lastMatID = 0;

	Material::Material(Shader &sh) : shader(sh) {
		if (!maxTextures) {
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextures);
		}

		matID = lastMatID++;
	}

	int Material::AddTexture(const std::string &img, bool linear, bool mipmap, Texture::TextureType textureType) {
		if (textures.size() == maxTextures) return -1;

		textures.push_back(std::make_unique<Texture>(img, linear, mipmap, textureType));

		return static_cast<int>(textures.size()) - 1;
	}
	void Material::RemoveTexture(int id) {
		if (id < 0 || id >= textures.size()) return;

		textures.erase(textures.begin() + id);
	}

	const Texture &Material::GetTexture(int id) const {
		if (id < 0 || id >= textures.size()) throw std::exception("Invalid texture ID");

		return *textures[id].get();
	}

	void Material::Use() {
		if (currMatID == matID) return;
		currMatID = matID;

		for (size_t i = 0; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			textures[i]->Use();
		}
	}
}