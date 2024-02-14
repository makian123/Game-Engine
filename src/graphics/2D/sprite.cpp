#include "graphics/2D/sprite.h"
#include <glm/ext.hpp>

#include <engine.h>
#include <graphics/camera.h>
#include <ecs/gameobject.h>

namespace engine::graphics {
	Sprite::Sprite(Material &mat_, const glm::vec4 &rectangle, Shader &sh, SDL_Color &&col_)
		:mat(mat_), rect(rectangle), shader(sh), col(col_), Drawable() {
		Vertex data[4];
		data[0] = {
			.pos = {rect[0], rect[1], 1},
			.uv = {0.0f, 0.0f}
		};
		data[1] = {
			.pos = {rect[0] + rect[2], rect[1], 1},
			.uv = {1.0f, 0.0f}
		};
		data[2] = {
			.pos = {rect[0], rect[1] + rect[3], 1},
			.uv = {0.0f, 1.0f}
		};
		data[3] = {
			.pos = {rect[0] + rect[2], rect[1] + rect[3], 1},
			.uv = {1.0f, 1.0f}
		};

		for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
			data[i].cols[0] = col.r / 255.0f;
			data[i].cols[1] = col.g / 255.0f;
			data[i].cols[2] = col.b / 255.0f;
			data[i].cols[3] = col.a / 255.0f;
		}

		const GLint indices[6] = {
			0, 1, 3,
			0, 3, 2
		};

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		GLint posAttrib = shader.GetAttrib("iPos");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(posAttrib);

		GLint colAttrib = shader.GetAttrib("iCol");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, cols)));
		glEnableVertexAttribArray(colAttrib);

		GLint uvAttrib = shader.GetAttrib("iUV");
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));
		glEnableVertexAttribArray(uvAttrib);

		GLint normAttrib = shader.GetAttrib("iNorm");
		glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(normAttrib);
	}
	Sprite::Sprite(const Sprite &other): shader(other.shader), rect(other.rect), mat(other.mat), col(other.col) {
		Vertex data[4];
		data[0] = {
			.pos = {rect[0], rect[1], 1},
			.uv = {0.0f, 0.0f},
			.normal = { 0, 0, -1 }
		};
		data[1] = {
			.pos = {rect[0] + rect[2], rect[1], 1},
			.uv = {1.0f, 0.0f},
			.normal = { 0, 0, -1 }
		};
		data[2] = {
			.pos = {rect[0], rect[1] + rect[3], 1},
			.uv = {0.0f, 1.0f},
			.normal = { 0, 0, -1 }
		};
		data[3] = {
			.pos = {rect[0] + rect[2], rect[1] + rect[3], 1},
			.uv = {1.0f, 1.0f},
			.normal = { 0, 0, -1 }
		};

		for (size_t i = 0; i < sizeof(data) / sizeof(data[0]); ++i) {
			data[i].cols[0] = col.r / 255.0f;
			data[i].cols[1] = col.g / 255.0f;
			data[i].cols[2] = col.b / 255.0f;
			data[i].cols[3] = col.a / 255.0f;
		}

		const GLint indices[6] = {
			0, 1, 3,
			0, 3, 2
		};

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		GLint posAttrib = shader.GetAttrib("iPos");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(posAttrib);

		GLint colAttrib = shader.GetAttrib("iCol");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, cols)));
		glEnableVertexAttribArray(colAttrib);

		GLint uvAttrib = shader.GetAttrib("iUV");
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, uv)));
		glEnableVertexAttribArray(uvAttrib);

		GLint normAttrib = shader.GetAttrib("iNorm");
		glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(normAttrib);
	}
	Sprite::~Sprite(){}

	std::array<Sprite::Vertex, 4> Sprite::GetVertexData() const {
		physics::Transform::Position position;
		if (gameobject) {
			position = gameobject->GetTransform().GetPos();
		}
		else {
			position = physics::Transform::Position{ rect[0], rect[1], 0.0f };
		}

		return { {
			// BOTTOM LEFT
			{
			.pos = {position.x - rect[2] / 2.0f, position.y - rect[3] / 2.0f, position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f}
			},
			// BOTTOM RIGHT
			{
			.pos = {position.x + rect[2] / 2.0f, position.y - rect[3] / 2.0f, position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f}
			},
			// TOP LEFT
			{
			.pos = {position.x - rect[2] / 2.0f, position.y + rect[3] / 2.0f, position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f}
			},
			// TOP RIGHT
			{
			.pos = {position.x + rect[2] / 2.0f, position.y + rect[3] / 2.0f, position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f}
			}
		} };

		/*
		return {{
			// BOTTOM LEFT
			{
			.pos = {rect[0] / 2.0f + (position.x / 2.0f), rect[1] + (position.y / 2.0f), position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 0.0f}
			},
			// BOTTOM RIGHT
			{
			.pos = {rect[0] + rect[2] + (position.x / 2.0f), rect[1] + (position.y / 2.0f), position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 0.0f}
			},
			// TOP LEFT
			{
			.pos = {rect[0] + (position.x / 2.0f), rect[1] + rect[3] + (position.y / 2.0f), position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {0.0f, 1.0f}
			},
			// TOP RIGHT
			{
			.pos = {rect[0] + rect[2] + (position.x / 2.0f), rect[1] + rect[3] + (position.y / 2.0f), position.z},
			.cols = {1.0f, 1.0f, 1.0f, 1.0f},
			.uv = {1.0f, 1.0f}
			}
		}};
		*/
	}

	void Sprite::Draw(const Camera &cam) const {
		auto data = GetVertexData();
		auto pos = gameobject->GetTransform().GetPos();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), &data.front(), GL_DYNAMIC_DRAW);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0, 0, 0));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));

		glm::mat4 projection = cam.GetMat();

		shader.Use();
		mat.Use();

		glUniformMatrix4fv(shader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shader.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shader.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//glUniform1f(shader.GetUniform("iTime"), engine::timer.TimePassed());

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	void Sprite::SetPos(const glm::vec2 &pos) {
		rect[0] = pos[0];
		rect[1] = pos[1];
	}
	void Sprite::SetDims(const glm::vec2 &dims) {
		rect[2] = dims[0];
		rect[3] = dims[1];
	}
}