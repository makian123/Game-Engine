#include "engine.h"
#include "graphics/camera.h"
#include <ecs/gameobject.h>
#include <ecs/scene.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace engine::graphics {
	static UUID currID;

	Camera::Camera(const glm::vec2 &dims_, bool ortho_):
		dims(dims_), ortho(ortho_), Component() {
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		fwd = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	void Camera::Start() {
		auto pos = gameobject->GetTransform().GetPos();
	}
	
	void Camera::Awake() {
		const_cast<Scene*>(gameobject->GetScene())->SetCamera(this);
	}
	void Camera::Update() {
		if (lastTransform != gameobject->GetTransform()) {
			updated = true;
		}
	}
	void Camera::LateUpdate() {
		if (updated) {
			lastTransform = gameobject->GetTransform();
		}
	}

	glm::mat4 Camera::GetMat() const {
		glm::mat4 ret;
		auto &transform = gameobject->GetTransform();

		if (ortho) {
			ret = glm::ortho<float>(
				transform.GetPos().x - dims.x / 2.0f * scale, transform.GetPos().x + dims.x / 2.0f * scale,
				transform.GetPos().y - dims.y / 2.0f * scale, transform.GetPos().y + dims.y / 2.0f * scale
			);
		}
		else {
			ret = glm::perspective(glm::radians(fov), dims.x / dims.y, 0.1f, 1000.0f);
		}

		return ret;
	}
	glm::mat4 Camera::GetProjMat() const {
		glm::mat4 ret;
		auto &transform = gameobject->GetTransform();

		if (ortho) {
			ret = glm::ortho<float>(
				-dims.x / 2.0f * scale,dims.x / 2.0f * scale,
				-dims.y / 2.0f * scale, dims.y / 2.0f * scale
			);
		}
		else {
			ret = glm::perspective(fov, dims.x / dims.y, 0.1f, 1000.0f);
		}

		return ret;
	}
	glm::mat4 Camera::GetViewMat() const {
		glm::mat4 ret = glm::mat4(1.0f);
		auto &transform = gameobject->GetTransform();
		
		ret *= glm::inverse(glm::mat4_cast(transform.GetRot()));
		ret = glm::translate(ret, -glm::vec3(transform.GetPos().x, transform.GetPos().y, transform.GetPos().z));
		ret = glm::scale(ret, glm::vec3(1, 1, 1));

		return ret;
	}

	void Camera::Use() const {
		if (currID == uuid) return;
		currID = uuid;

		auto &pos = gameobject->GetTransform().GetPos();
		if (ortho) { 
			glOrtho(pos.x, pos.x + dims.x, pos.y, pos.y + dims.y, 0, 1000);
			return;
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}
}