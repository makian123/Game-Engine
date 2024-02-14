#pragma once

#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <ecs/component.h>
#include <physics/transform.h>
#include <GL/glew.h>
#include <SDL/SDL_opengl.h>

namespace engine::graphics {
	class Camera: public Component {
	private:
		bool ortho;
		glm::vec2 dims;
		glm::vec3 up, fwd;
		float fov = 60.0f;
		float scale = 1.0f;
		mutable bool updated = true;
		physics::Transform lastTransform;

	public:
		Camera() = default;
		Camera(const glm::vec2 &dims_, bool ortho_ = false);

		void Start() override;
		void Awake() override;
		void Update() override;
		void LateUpdate() override;

		glm::vec2 GetDims() const { return dims; }
		float GetFOV() const { return fov; }
		float GetScale() const { return scale; }
		glm::mat4 GetMat() const;
		glm::mat4 GetProjMat() const;
		glm::mat4 GetViewMat() const;
		bool IsOrtho() const { return ortho; }

		void Use() const;

		void SetDims(const glm::vec2 &dims_) { dims = dims_; }
		void SetFOV(float fov_) { fov = std::clamp<float>(fov_, 10.0f, 180.0f); }
		void SetScale(float scale_) { scale = scale_; }
		void SetOrtho(bool isOrtho) { ortho = isOrtho; if (!ortho) { glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LESS); } }
	};
}