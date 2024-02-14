#pragma once

#include <ecs/component.h>
#include <physics/transform.h>
#include "shader.h"

namespace engine::graphics {
	class PointLight : public Component {
		physics::Transform::Position lastPos;
		physics::Transform::Position currPos;
		Shader &shader;

		public:
		PointLight(Shader &sh) : shader(sh) {}
		~PointLight() {}

		void Start() override;
		void Update() override;
	};
}