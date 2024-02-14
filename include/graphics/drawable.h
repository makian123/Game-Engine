#pragma once

#include "camera.h"
#include <GL/glew.h>
#include <SDL/SDL_opengl.h>
#include <glm/common.hpp>

namespace engine::graphics {
	class Drawable {
	protected:
		GLuint vbo = 0, vao = 0, ebo = 0;
	public:
		Drawable() {
			glGenBuffers(1, &vbo);
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &ebo);
		}
		virtual ~Drawable() {
			glDeleteBuffers(1, &ebo);
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
		}

		virtual void Draw(const Camera &cam) const = 0;
	};
}