#include "graphics/3D/mesh.h"
#include <vector>
#include <sstream>
#include <array>
#include <cstring>
#include <fstream>
#include <cmath>
#include <glm/ext.hpp>
#include <ecs/gameobject.h>
#include <physics/transform.h>
#include "engine.h"

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
	class ObjLoader {
		private:
		std::vector<engine::graphics::Mesh::Vertex> verts;
		std::vector<int> indices;
		int triangleCount = 0;

		std::vector<std::string> SplitStr(std::string s, std::string delimiter) {
			size_t pos_start = 0, pos_end, delim_len = delimiter.length();
			std::string token;
			std::vector<std::string> res;

			while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
				token = s.substr(pos_start, pos_end - pos_start);
				pos_start = pos_end + delim_len;
				res.push_back(token);
			}

			res.push_back(s.substr(pos_start));
			return res;
		}

		public:
		ObjLoader(std::ifstream &file) {
			std::string line;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> uvs;

			while (std::getline(file, line)) {
				auto parts = SplitStr(line, " ");

				if (parts[0] == "v") {
					vertices.emplace_back(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]));
				}
				else if (parts[0] == "vn") {
					normals.emplace_back(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]));
				}
				else if (parts[0] == "vt") {
					uvs.emplace_back(std::stof(parts[1]), std::stof(parts[2]));
				}
				else if (parts[0] == "f") {
					for (size_t i = 1; i < parts.size(); ++i) {
						auto split = SplitStr(parts[i], "/");
						std::array<int, 3> vertInfo;

						glm::vec3 &vecPos = vertices[std::stoi(split[0]) - 1];
						glm::vec2 &vertUV = uvs[std::stoi(split[1]) - 1];
						glm::vec3 &vertNorm = normals[std::stoi(split[2]) - 1];

						verts.emplace_back(Mesh::Vertex{
							{ vecPos[0], vecPos[1], vecPos[2] },
							{ 1.0, 1.0, 1.0, 1.0 },
							{ vertUV[0], vertUV[1] },
							{ vertNorm[0], vertNorm[1], vertNorm[2] }
							});

						if (parts.size() - 1 == 3) {
							indices.emplace_back(indices.size());
							triangleCount++;
						}
						else if (parts.size() - 1 == 4) {
							if (i == 4) {
								size_t beginIdx = (triangleCount / 2) * 4;
								indices.emplace_back(beginIdx + 0);
								indices.emplace_back(beginIdx + 1);
								indices.emplace_back(beginIdx + 2);
								indices.emplace_back(beginIdx + 0);
								indices.emplace_back(beginIdx + 2);
								indices.emplace_back(beginIdx + 3);

								triangleCount += 2;
							}
						}
					}
				}
			}
		}
		~ObjLoader() = default;

		const std::vector<engine::graphics::Mesh::Vertex> &GetVerts() const { return verts; }
		const std::vector<int> &GetIndices() const { return indices; }
		int GetTriangleCount() const { return triangleCount; }
	};

	Mesh::Mesh(const std::string &data, Shader &sh, Material &mat_) : mat(mat_), shader(sh), modelMatrix(glm::mat4(1.0f)), Drawable() {
		std::ifstream file(data);
		ObjLoader object(file);
		file.close();
		/*
		vertices = {
			{-0.5f,	-0.5f,	0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.25f,	0.66f},	//0,0,0
			{-0.5f,	0.5f,	0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.50f,	0.66f},	//0,1,0
			{0.5f,	-0.5f,	0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.25f,	0.33f},	//1,0,0
			{0.5f,	0.5f,	0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.50f,	0.33f},	//1,1,0

			{-0.5f,	-0.5f,	-0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.25f,	0.66f},	//0,0,1
			{-0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.50f,	0.66f},	//0,1,1
			{0.5f,	-0.5f,	-0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.25f,	0.33f},	//1,0,1
			{0.5f,	0.5f,	-0.5f,		1.0f,	1.0f,	1.0f,	1.0f,		0.50f,	0.33f}	//1,1,1
		};

		std::array<int, 6> tris;
		std::vector<int> indices;

		tris = FaceToVert({ 0, 1, 2, 3 });	//FRONT
		std::copy(tris.begin(), tris.end(), std::back_inserter(indices));
		tris = FaceToVert({ 1, 3, 5, 7 });	//UP
		std::copy(tris.begin(), tris.end(), std::back_inserter(indices));
		tris = FaceToVert({ 0, 1, 4, 5 });	//LEFT
		std::copy(tris.begin(), tris.end(), std::back_inserter(indices));
		tris = FaceToVert({ 0, 2, 4, 6 });	//DOWN
		std::copy(tris.begin(), tris.end(), std::back_inserter(indices));
		tris = FaceToVert({ 2, 3, 6, 7 });	//RIGHT
		std::copy(tris.begin(), tris.end(), std::back_inserter(indices));
		tris = FaceToVert({ 4, 5, 6, 7 });	//BACK
		std::copy(tris.begin(), tris.end(), std::back_inserter(indices));
		faceCount = 6;
		*/

		object.GetVerts();
		this->trisCount = object.GetTriangleCount();

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, object.GetVerts().size() * sizeof(Vertex), object.GetVerts().data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.GetIndices().size() * sizeof(int), object.GetIndices().data(), GL_DYNAMIC_DRAW);

		GLint posAttrib = shader.GetAttrib("iPos");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
		glEnableVertexAttribArray(posAttrib);

		GLint colAttrib = shader.GetAttrib("iCol");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, col));
		glEnableVertexAttribArray(colAttrib);

		GLint uvAttrib = shader.GetAttrib("iUV");
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
		glEnableVertexAttribArray(uvAttrib);

		GLint normAttrib = shader.GetAttrib("iNorm");
		glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
		glEnableVertexAttribArray(normAttrib);

		file.close();
	}

	void Mesh::Draw(const Camera &cam) const {
		shader.Use();
		mat.Use();

		glm::mat4 view = cam.GetViewMat();
		glm::mat4 projection = cam.GetProjMat();

		glUniformMatrix4fv(shader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(shader.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(shader.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(vao);

		// FaceCount * 6 is number of triangles with duplicated vertices
		glDrawElements(GL_TRIANGLES, trisCount * 3, GL_UNSIGNED_INT, nullptr);
	}
	void Mesh::Update() {
		if (gameobject->GetTransform() != lastTransform) {
			updated = true;
		}
		if (!updated) return;

		modelMatrix = glm::mat4(1.0f);

		auto scaleMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
		glm::mat4 rotationMatrix = modelMatrix * glm::mat4_cast(gameobject->GetTransform().GetRot());
		auto translationMatrix = glm::translate(
			modelMatrix,
			glm::vec3(gameobject->GetTransform().GetPos().x, gameobject->GetTransform().GetPos().y, gameobject->GetTransform().GetPos().z)
		);

		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}
	void Mesh::LateUpdate() {
		if (updated) {
			lastTransform = gameobject->GetTransform();
		}
	}
}