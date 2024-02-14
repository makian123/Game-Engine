#pragma once

#include "../drawable.h"
#include "../shader.h"
#include "../2D/sprite.h"
#include "../material.h"
#include <ecs/component.h>

#include <glm/vec3.hpp>
#include <memory>

namespace engine::graphics {
	class Cube : public Component, public Drawable {
		private:
		Shader &sh;
		glm::vec3 dims;	// width, height, depth
		Material &mat;
		bool updated = true;

		std::array<Sprite::Vertex, 24> GetVerts() const;
		std::vector<GLint> GetIndices() const;
		public:
		Cube(const glm::vec3 &dims, Shader &shader, Material &mat);

		void Draw(const Camera &cam) const override;
		void LateUpdate() override;

	};
}