#pragma once

#include <string>
#include <vector>
#include <memory>
#include <graphics/drawable.h>
#include <graphics/shader.h>
#include <graphics/material.h>
#include <physics/transform.h>
#include <ecs/component.h>
#include <glm/glm.hpp>

namespace engine::graphics {
	class Mesh : public Component, public Drawable {
	public:
		struct Vertex {
			GLfloat pos[3];
			GLfloat col[4];
			GLfloat uv[2];
			GLfloat normal[3];
		};
	private:
		Shader &shader;
		std::vector<Vertex> vertices;
		Material &mat;
		int trisCount = 0;
		mutable bool updated = false;
		glm::mat4 modelMatrix;
		physics::Transform lastTransform;

	public:
		Mesh() = default;
		Mesh(const std::string &data, Shader &sh, Material &mat);

		void Draw(const Camera& cam) const override;
		void Update() override;
		void LateUpdate() override;
	};
}