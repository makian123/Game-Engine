#pragma once

#include <glm/glm.hpp>

namespace engine::physics {
	struct Quaternion {
		float x, y, z, w;

		Quaternion() = default;
		Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Quaternion(const Quaternion &other) : x(other.x), y(other.y), z(other.z), w(other.w) {}

		static Quaternion EulerAngles(double roll, double pitch, double yaw) {
            double cr = glm::cos(roll * 0.5);
            double sr = glm::sin(roll * 0.5);
            double cp = glm::cos(pitch * 0.5);
            double sp = glm::sin(pitch * 0.5);
            double cy = glm::cos(yaw * 0.5);
            double sy = glm::sin(yaw * 0.5);

            Quaternion q;
            q.w = cr * cp * cy + sr * sp * sy;
            q.x = sr * cp * cy - cr * sp * sy;
            q.y = cr * sp * cy + sr * cp * sy;
            q.z = cr * cp * sy - sr * sp * cy;

            return q;
		}
	};
}