#include "graphics/shader.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace engine::graphics {
	Shader::Shader(const std::string &vert, const std::string &frag, bool file) {
		program = glCreateProgram();

		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);

		GLchar *vertStr;
		if (!file) {
			vertStr = new char[vert.length() + 1];
			memset(vertStr, 0, vert.length() + 1);

			memcpy(vertStr, vert.c_str(), vert.length() + 1);
		}
		else {
			std::ifstream fileIn(vert);

			size_t sz = std::filesystem::file_size(vert) + 1;
			vertStr = new char[sz];
			memset(vertStr, 0, sz);

			fileIn.read(vertStr, sz);

			fileIn.close();
		}

		glShaderSource(vertex, 1, &vertStr, nullptr);
		glCompileShader(vertex);

		GLint result;
		char log[512] = { 0 };
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
		if (result != GL_TRUE) {
			std::cout << "Error compiling vertex shader:\n";
			glGetShaderInfoLog(vertex, sizeof(log), nullptr, log);
			std::cout << log << "\n";

			glDeleteShader(vertex);
			delete[] vertStr;
			throw new std::exception("Invalid parameter");
		}
		glAttachShader(program, vertex);

		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		GLchar *fragStr;
		if (!file) {
			fragStr = new char[frag.length() + 1];
			memset(fragStr, 0, frag.length() + 1);

			memcpy(fragStr, frag.c_str(), frag.length() + 1);
		}
		else {
			std::ifstream fileIn(frag);

			size_t sz = std::filesystem::file_size(frag) + 1;
			fragStr = new char[sz];
			memset(fragStr, 0, sz);

			fileIn.read(fragStr, sz);

			fileIn.close();
		}
		glShaderSource(fragment, 1, &fragStr, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);
		if (result != GL_TRUE) {
			std::cout << "Error compiling fragment shader:\n";
			glGetShaderInfoLog(fragment, sizeof(log), nullptr, log);
			std::cout << log << "\n";

			glDeleteShader(vertex);
			glDeleteShader(fragment);
			delete[] vertStr;
			delete[] fragStr;
			throw new std::exception("Invalid parameter");
		}
		glAttachShader(program, fragment);


		glLinkProgram(program);
		GLint programSuccess = GL_TRUE;
		glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE) {
			std::cout << "Error linking program:\n";
			glGetProgramInfoLog(program, sizeof(log), nullptr, log);
			std::cout << log << "\n";

			glDeleteShader(vertex);
			glDeleteShader(fragment);
			delete[] vertStr;
			delete[] fragStr;
			throw new std::exception("err");
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		delete[] vertStr;
		delete[] fragStr;

		glUseProgram(program);

		glUniform1i(GetUniform("colorMap"), 0);
		glUniform1i(GetUniform("normalMap"), 1);
		glUniform1i(GetUniform("specularMap"), 2);
	}
	Shader::~Shader() {
		glDeleteProgram(program);
	}

	GLint Shader::GetAttrib(const std::string &attr) {
		return glGetAttribLocation(program, attr.c_str());
	}
	GLint Shader::GetUniform(const std::string &unif) {
		return glGetUniformLocation(program, unif.c_str());
	}
}