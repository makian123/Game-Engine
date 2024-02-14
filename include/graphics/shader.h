#pragma once

#include <string>
#include <GL/glew.h>

namespace engine::graphics {
	class Shader {
	private:
		GLuint program = 0;

	public:
		Shader() = default;
		Shader(const std::string &vert, const std::string &frag, bool path = false);
		~Shader();

		void Use() { glUseProgram(program); }
		GLint GetAttrib(const std::string &attr);
		GLint GetUniform(const std::string &unif);
		GLuint GetProgram() const { return program; }
	};
}