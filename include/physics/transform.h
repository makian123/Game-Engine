#pragma once

#include <ecs/component.h>
#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace engine { namespace physics {
	class Transform : public Component {
		public:
		struct Position {
			float x = 0, y = 0, z = 0;

			Position() = default;
			Position(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
			Position(const Position &other) : x(other.x), y(other.y), z(other.z) {}

			bool operator==(const Position &other) const  {
				return x == other.x && y == other.y && z == other.z;
			}
			bool operator !=(const Position &other) const { 
				return !(*this == other);
			}

			Position operator+(const Position &other) const {
				return Position(this->x + other.x, this->y + other.y, this->z + other.z);
			}
			Position &operator+=(const Position &other) {
				this->x += other.x;
				this->y += other.y;
				this->z += other.z;

				return *this;
			}
			Position operator-(const Position &other) const {
				return Position(this->x - other.x, this->y - other.y, this->z - other.z);
			}
			Position &operator-=(const Position &other) {
				this->x -= other.x;
				this->y -= other.y;
				this->z -= other.z;

				return *this;
			}
		};

		private:
		b2Transform transform;
		Position pos;
		glm::quat rot;

		glm::vec3 front, up, right;

		public:
		Transform();
		explicit Transform(float x, float y, float z);
		explicit Transform(const b2Transform &transform_);
		~Transform() = default;

		const Position &GetPos() const { return pos; }
		const glm::quat &GetRot() const { return rot; }
		const b2Transform &GetTransform2D() const { return transform; }

		Transform &operator=(const Transform &other);
		bool operator==(const Transform &other) const {
			return (this->pos == other.pos) && (this->rot == other.rot);
		}
		bool operator!=(const Transform &other) const {
			return !(*this == other);
		}

		void SetPos(const Position &pos_) {
			pos = pos_;
			transform.p = b2Vec2(pos.x, pos.y);
		}
		void SetPos(float x_, float y_, float z_) {
			pos.x = x_;
			pos.y = y_;
			pos.z = z_;

			transform.p = b2Vec2(pos.x, pos.y);
		}

		void SetRot(const glm::quat &rot_);
		void SetRot(float x_, float y_, float z_);

		void SetTransform(const Transform &other);
		void SetTransform(const b2Transform &other);


		const glm::vec3 &Forward() const { return front; }
		const glm::vec3 &Up() const { return up; }
		const glm::vec3 &Right() const { return right; }
	};
}}