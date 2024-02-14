#include <physics/transform.h>

namespace engine::physics {
	Transform::Transform() : rot(1, 0, 0, 0) {
		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);
	}
	 Transform::Transform(float x, float y, float z) : rot(1, 0, 0, 0) {
		pos = Position{ x, y, z };
		transform.p = b2Vec2(x, y);

		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);
	}
	Transform::Transform(const b2Transform &transform_) : rot(1, 0, 0, 0) {
		transform = transform_;
		pos.x = transform.p.x;
		pos.y = transform.p.y;
		pos.z = 0.0f;

		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);
	}

	void Transform::SetRot(const glm::quat &rot_) {
		rot = rot_;

		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);
	}
	void Transform::SetRot(float x_, float y_, float z_) {
		rot = glm::quat(glm::vec3(x_, y_, z_));

		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);
	}

	void Transform::SetTransform(const Transform &other) {
		transform = other.transform;
		pos = other.pos;

		rot = other.rot;

		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);
	}
	void Transform::SetTransform(const b2Transform &other) {
		transform = other;
		pos = {
			other.p.x,
			other.p.y,
			0.0f
		};

		rot = glm::quat(glm::vec3(0, 0, glm::degrees(other.q.GetAngle())));

		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);
	}

	Transform &Transform::operator=(const Transform &other) {
		this->transform = other.transform;
		this->gameobject = other.gameobject;

		this->pos = other.pos;
		this->rot = other.rot;

		this->up = rot * glm::vec3(0, 1, 0);
		this->right = rot * glm::vec3(1, 0, 0);
		this->front = rot * glm::vec3(0, 0, 1);

		return *this;
	}
}