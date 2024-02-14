#include <graphics/3D/cube.h>
#include <ecs/gameobject.h>

#include <glm/ext.hpp>

static std::array<int, 6> FaceToVert(const std::array<int, 4> &face) {
	std::array<int, 6> ret;

	ret.at(0) = face.at(0);
	ret.at(1) = face.at(1);
	ret.at(2) = face.at(2);

	ret.at(3) = face.at(2);
	ret.at(4) = face.at(3);
	ret.at(5) = face.at(1);

	return ret;
}

namespace engine::graphics {
	Cube::Cube(const glm::vec3 &dims, Shader &shader, Material &mat_): dims(dims), sh(shader), mat(mat_)  {
		std::vector<GLint> indices = GetIndices();

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(decltype(indices)::value_type), indices.data(), GL_DYNAMIC_DRAW);

		GLint posAttrib = sh.GetAttrib("iPos");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), nullptr);
		glEnableVertexAttribArray(posAttrib);

		GLint colAttrib = sh.GetAttrib("iCol");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void *)(offsetof(Sprite::Vertex, cols)));
		glEnableVertexAttribArray(colAttrib);

		GLint uvAttrib = sh.GetAttrib("iUV");
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void *)(offsetof(Sprite::Vertex, uv)));
		glEnableVertexAttribArray(uvAttrib);

		GLint normAttrib = sh.GetAttrib("iNorm");
		glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void *)(offsetof(Sprite::Vertex, normal)));
		glEnableVertexAttribArray(normAttrib);
	}

	void Cube::Draw(const Camera &cam) const {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		sh.Use();
		mat.Use();

		if (updated) {
			auto vertices = GetVerts();
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(decltype(vertices)::value_type), vertices.data(), GL_DYNAMIC_DRAW);
		}

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0, 0, 0));

		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 projection = cam.GetMat();

		glUniformMatrix4fv(sh.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(sh.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(sh.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
		auto &camPos = gameobject->GetScene()->GetCamera()->GetGameObject()->GetTransform().GetPos();
		glUniform3f(sh.GetUniform("camPos"), camPos.x, camPos.y, camPos.z);

		glUniform1f(sh.GetUniform("iTime"), engine::timer.TimePassed());

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	}
	void Cube::LateUpdate() {
		updated = false;
	}

	std::array<Sprite::Vertex, 24> Cube::GetVerts() const {
		auto &pos = gameobject->GetTransform().GetPos();
		std::array<Sprite::Vertex, 24> ret;

		// Front verts
		ret[0] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.25f, 0.345f },
			.normal = { 0.0f, 0.0f, 1.0f }
		};
		ret[1] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.495f, 0.345f },
			.normal = { 0.0f, 0.0f, 1.0f }
		};
		ret[2] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.25f, 0.666f },
			.normal = { 0.0f, 0.0f, 1.0f }
		};
		ret[3] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.495f, 0.666f },
			.normal = { 0.0f, 0.0f, 1.0f }
		};

		// Up verts
		ret[4] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.25f, 0.666f },
			.normal = { 0.0f, 1.0f, 0.0f }
		};
		ret[5] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.495f, 0.666f },
			.normal = { 0.0f, 1.0f, 0.0f }
		};
		ret[6] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.25f, 1.0f },
			.normal = { 0.0f, 1.0f, 0.0f }
		};
		ret[7] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.495f, 1.0f },
			.normal = { 0.0f, 1.0f, 0.0f }
		};

		// Back verts
		ret[8] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.75f, 0.335f },
			.normal = { 0.0f, 0.0f, -1.0f }
		};
		ret[9] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 1.0f, 0.335f },
			.normal = { 0.0f, 0.0f, -1.0f }
		};
		ret[10] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.75f, 0.666f },
			.normal = { 0.0f, 0.0f, -1.0f }
		};
		ret[11] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 1.0f, 0.666f },
			.normal = { 0.0f, 0.0f, -1.0f }
		};

		// Down verts
		ret[12] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.252f, 0.335f },
			.normal = { 0.0f, -1.0f, 0.0f }
		};
		ret[13] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.50f, 0.335f },
			.normal = { 0.0f, -1.0f, 0.0f }
		};
		ret[14] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.252f, 0.0f },
			.normal = { 0.0f, -1.0f, 0.0f }
		};
		ret[15] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.50f, 0.0f },
			.normal = { 0.0f, -1.0f, 0.0f }
		};

		// Left verts
		ret[16] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.0f, 0.335f },
			.normal = { -1.0f, 0.0f, 0.0f }
		};
		ret[17] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.25f, 0.335f },
			.normal = { -1.0f, 0.0f, 0.0f }
		};
		ret[18] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.0f, 0.666f },
			.normal = { -1.0f, 0.0f, 0.0f }
		};
		ret[19] = Sprite::Vertex{
			.pos = { pos.x - dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.25f, 0.666f },
			.normal = { -1.0f, 0.0f, 0.0f }
		};

		// Right verts
		ret[20] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.50f, 0.34f },
			.normal = { 1.0f, 0.0f, 0.0f }
		};
		ret[21] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y - dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.75f, 0.34f },
			.normal = { 1.0f, 0.0f, 0.0f }
		};
		ret[22] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z + dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.50f, 0.66f },
			.normal = { 1.0f, 0.0f, 0.0f }
		};
		ret[23] = Sprite::Vertex{
			.pos = { pos.x + dims.x / 2.0f, pos.y + dims.y / 2.0f, pos.z - dims.z / 2.0f},
			.cols = { 1.0f, 1.0f, 1.0f, 1.0f },
			.uv = { 0.75f, 0.66f },
			.normal = { 1.0f, 0.0f, 0.0f }
		};

		return ret;
	}
	std::vector<GLint> Cube::GetIndices() const {
		std::vector<GLint> ret = {
			0, 1, 3,		0, 3, 2,
			4, 5, 7,		4, 7, 6,
			8, 9, 11,		8, 11, 10,
			12, 13, 15,		12, 15, 14,
			16, 17, 19,		16, 19, 18,
			20, 21, 23,		20, 23, 22
		};

		return ret;
	}
}