#pragma once

#include <string>
#include <memory>
#include <array>
#include <SDL/SDL.h>
#include <glm/vec4.hpp>
#include <GL/glew.h>
#include "../drawable.h"
#include "../shader.h"
#include "../camera.h"
#include "../texture.h"
#include "../material.h"
#include <ecs/component.h>

namespace engine::graphics {
	class Sprite: public Component, public Drawable {
	public:
		__pragma(pack(push, 1))
		struct Vertex {
			float pos[3] = { 0 };
			float cols[4] = { 0 };
			float uv[2] = { 0 };
			float normal[3] = { 0 };
		};
		__pragma(pack(pop))
	private:
		glm::vec4 rect = { 0, 0, 0, 0 };
		Shader &shader;
		SDL_Color col;
		Material &mat;

		std::array<Sprite::Vertex, 4> GetVertexData() const;
	public:
		friend class Tilemap;
		Sprite(Material &mat, const glm::vec4 &rectangle, Shader &sh, SDL_Color &&col = { 255, 255, 255, 255 });
		Sprite(const Sprite &other);
		~Sprite();

		void Draw(const Camera &cam) const override;

		// Sets position relative to the GameObject
		void SetPos(const glm::vec2 &pos);
		glm::vec2 GetPos() const { return glm::vec2{ rect.x, rect.y }; }

		// Sets dimensions of the sprite
		void SetDims(const glm::vec2 &dims);
		glm::vec2 GetDims() const { return glm::vec2{ rect.z, rect.w }; }
	};
}