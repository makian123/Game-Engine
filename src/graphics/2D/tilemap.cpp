#include <graphics/2D/tilemap.h>
#include <glm/ext.hpp>

namespace engine::graphics {
	Tilemap::Tilemap(float tileWidth, float tileHeight, float offsetX_, float offsetY_)
		: tileW(tileWidth), tileH(tileHeight), offsetX(offsetX_), offsetY(offsetY_) {
	}

	Tilemap::~Tilemap() {
		for (auto &[idx, buffer]: buffers) {
			glDeleteBuffers(1, &std::get<0>(buffer));
			glDeleteBuffers(1, &std::get<1>(buffer));
			glDeleteBuffers(1, &std::get<2>(buffer));
		}
	}

	void Tilemap::Draw(const Camera &cam) const {
		auto &objPos = gameobject->GetTransform().GetPos();

		int currSpriteIdx = -1;
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0, 0, 0));

		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 projection = cam.GetMat();

		for (auto &[idx, vector] : tilesPos) {
			auto &idcs = indices.at(idx);
			auto &buff = buffers.at(idx);

			glBindVertexArray(std::get<0>(buff));
			glBindBuffer(GL_ARRAY_BUFFER, std::get<1>(buff));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, std::get<2>(buff));

			auto &sprite = sprites.at(idx);

			sprite.shader.Use();
			sprite.mat.Use();


			glUniformMatrix4fv(sprite.shader.GetUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(sprite.shader.GetUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(sprite.shader.GetUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glUniform1f(sprite.shader.GetUniform("iTime"), engine::timer.TimePassed());

			glDrawElements(GL_TRIANGLES, idcs.size(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void Tilemap::SetTile(int64_t x, int64_t y, int spriteIdx) {
		if (tiles.contains({ x, y })) {
			auto idx = tiles[{x, y}];
			if (idx == spriteIdx) return;

			tiles[{ x, y }] = spriteIdx;

			auto posIdx = std::find_if(tilesPos.at(idx).begin(), tilesPos.at(idx).end(), [x, y](const std::pair<int64_t, int64_t> &currPos) {
				return x == currPos.first && y == currPos.second;
			});
			if (posIdx != tilesPos.at(idx).end()) {
				tilesPos.at(idx).erase(posIdx);
			}
		}
		else {
			tiles[{ x, y }] = spriteIdx;
			tilesPos[spriteIdx].push_back({ x, y });
		}

		auto &sprite = sprites[spriteIdx];
		sprite.SetPos({ x * tileW + offsetX, y * tileH + offsetY });
		auto spriteVerts = sprite.GetVertexData();

		std::vector<Sprite::Vertex> verts;
		auto &idcs = indices[spriteIdx];

		if (!buffers.contains(spriteIdx)) {
			buffers[spriteIdx] = std::tuple<GLuint, GLuint, GLuint>(0, 0, 0);
			bufferInfo[spriteIdx] = { 0, BUFFER_START_CAP };
			glGenVertexArrays(1, &std::get<0>(buffers.at(spriteIdx)));
			glGenBuffers(1, &std::get<1>(buffers.at(spriteIdx)));
			glGenBuffers(1, &std::get<2>(buffers.at(spriteIdx)));

			glBindBuffer(GL_ARRAY_BUFFER, std::get<1>(buffers.at(spriteIdx)));
			glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite::Vertex) * bufferInfo[spriteIdx].second, nullptr, GL_STATIC_DRAW);
		}
		auto &buffs = buffers[spriteIdx];
		auto &[bufferSize, bufferCap] = bufferInfo[spriteIdx];

		size_t beginIdx = 0;
		if (fastTiles) {
			beginIdx = vertices[spriteIdx].size();
			for (auto &ver : spriteVerts) {
				vertices[spriteIdx].push_back(ver);
			}
		}
		else {
			beginIdx = (tilesPos[spriteIdx].size() - 1) * 4;
			for (auto &ver : spriteVerts) {
				verts.push_back(ver);
			}
		}
		idcs.push_back(beginIdx + 0);
		idcs.push_back(beginIdx + 1);
		idcs.push_back(beginIdx + 3);
		idcs.push_back(beginIdx + 0);
		idcs.push_back(beginIdx + 3);
		idcs.push_back(beginIdx + 2);

		glBindVertexArray(std::get<0>(buffs));

		glBindBuffer(GL_ARRAY_BUFFER, std::get<1>(buffs));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, std::get<2>(buffs));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * idcs.size(), idcs.data(), GL_STATIC_DRAW);

		GLint posAttrib = sprite.shader.GetAttrib("iPos");
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), nullptr);
		glEnableVertexAttribArray(posAttrib);

		GLint colAttrib = sprite.shader.GetAttrib("iCol");
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void *)(offsetof(Sprite::Vertex, cols)));
		glEnableVertexAttribArray(colAttrib);

		GLint uvAttrib = sprite.shader.GetAttrib("iUV");
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Sprite::Vertex), (void *)(offsetof(Sprite::Vertex, uv)));
		glEnableVertexAttribArray(uvAttrib);

		if (fastTiles) {
			bufferSize = bufferCap = vertices[spriteIdx].size();

			glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite::Vertex) * vertices[spriteIdx].size(), vertices[spriteIdx].data(), GL_STATIC_DRAW);
		}
		else {
			// Tries to get the buffer mapped into pointer
			Sprite::Vertex *ptr = (Sprite::Vertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
			if (!ptr) {
				std::cout << "Error getting buffer pointer\n";
				return;
			}

			// Calculates future buffer size
			size_t newBufferSize = bufferSize + verts.size();
			if (newBufferSize >= bufferCap) {
				// Gets old buffer
				Sprite::Vertex *oldData = new Sprite::Vertex[bufferSize];
				memcpy(oldData, ptr, sizeof(Sprite::Vertex) * bufferSize);

				// Increases capacity
				while (bufferCap <= newBufferSize)
					bufferCap *= 2;

				// Creates a new buffer and populates it with old data
				glUnmapBuffer(GL_ARRAY_BUFFER);
				glBufferData(GL_ARRAY_BUFFER, sizeof(Sprite::Vertex) *bufferCap, nullptr, GL_STATIC_DRAW);
				ptr = (Sprite::Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);

				memcpy(ptr, oldData, sizeof(Sprite::Vertex) *bufferSize);

				delete[] oldData;
			}

			// Appends new data to the buffer
			memcpy((void *)((uintptr_t)ptr + sizeof(Sprite::Vertex) * bufferSize), verts.data(), sizeof(Sprite::Vertex) * verts.size());

			// Updates buffer size
			bufferSize = newBufferSize;
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}
	}
	void Tilemap::RemoveTile(int64_t x, int64_t y) {
		if (!tiles.contains({ x, y })) return;
		
		auto spriteIdx = tiles[{x, y}];
		auto &inds = indices[spriteIdx];
		auto &positions = tilesPos[spriteIdx];
		size_t foundIdx = 0;
		for (size_t i = 0; i < positions.size(); ++i) {
			if (positions[i].first != x || positions[i].second != y) continue;
			foundIdx = i;
			positions.erase(positions.begin() + i);

			inds.erase(inds.begin() + i * 6, inds.begin() + i * 6 + 6);

			for (size_t j = i * 6; j < inds.size(); ++j) {
				inds[j] -= 4;
			}
			break;
		}

		auto &buffs = buffers[spriteIdx];
		auto &[bufferSize, bufferCap] = bufferInfo[spriteIdx];

		glBindVertexArray(std::get<0>(buffs));

		glBindBuffer(GL_ARRAY_BUFFER, std::get<1>(buffs));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, std::get<2>(buffs));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * inds.size(), inds.data(), GL_STATIC_DRAW);

		// Tries to get the buffer mapped into pointer
		Sprite::Vertex *ptr = (Sprite::Vertex *)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
		if (!ptr) {
			std::cout << "Error getting buffer pointer\n";
			return;
		}

		// Calculates future buffer size
		size_t newBufferSize = bufferSize - 4;

		for (size_t i = foundIdx; i < bufferSize - 1; ++i) {
			memcpy(&ptr[i * 4], &ptr[(i + 1) * 4], sizeof(Sprite::Vertex) * 4);
		}
		memset(&ptr[bufferSize - 1], 0, sizeof(Sprite::Vertex));

		// Updates buffer size
		bufferSize = newBufferSize;
		glUnmapBuffer(GL_ARRAY_BUFFER);

		tiles.erase({ x, y });
	}
	int Tilemap::GetTile(int64_t x, int64_t y) const {
		try {
			return tiles.at(std::pair{ x, y });
		}
		catch (...) {
			return 0;
		}
	}

	int Tilemap::AddSprite(const Sprite &sprite) {
		sprites.push_back(sprite);
		sprites.back().SetDims({ tileW, tileH });

		return sprites.size() - 1;
	}
}