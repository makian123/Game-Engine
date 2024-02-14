#pragma once

#include <unordered_map>
#include <tuple>
#include "sprite.h"
#include <ecs/component.h>
#include <ecs/gameobject.h>
#include <graphics/drawable.h>
#include <glm/vec2.hpp>

namespace engine::graphics {

	class Tilemap : public Component, public Drawable {
		private:
		struct pair_hash
		{
			template <class T1, class T2>
			std::size_t operator() (const std::pair<T1, T2> &pair) const
			{
				return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
			}
		};

		static constexpr size_t BUFFER_START_CAP = 8;

		std::unordered_map<std::pair<int64_t, int64_t>, int, pair_hash> tiles;
		std::unordered_map<int, std::vector<Sprite::Vertex>> vertices;
		std::unordered_map<int, std::vector<std::pair<int64_t, int64_t>>> tilesPos;
		std::unordered_map<int, std::vector<GLint>> indices;
		std::unordered_map<int, std::tuple<GLuint, GLuint, GLuint>> buffers;
		std::unordered_map<int, std::pair<size_t, size_t>> bufferInfo;

		std::vector<Sprite> sprites;
		float tileW, tileH;

		float offsetX = 0;
		float offsetY = 0;

		bool fastTiles = false;

		public:
		Tilemap(float tileWidth, float tileHeight, float offsetX = 0.0f, float offsetY = 0.0f);
		~Tilemap();
		void Draw(const Camera &cam) const override;

		void Begin() { fastTiles = true; }
		void End() { fastTiles = false; vertices.clear(); vertices.rehash(0); }

		/**
		* x - the x index on the tilemap (not world pos), relative to the tilemap gameobject
		* y - the y index on the tilemap (not world pos), relative to the tilemap gameobject
		* spriteIdx - got from `GetTile(x, y)`
		*/
		void SetTile(int64_t x, int64_t y, int spriteIdx);
		void RemoveTile(int64_t x, int64_t y);
		int GetTile(int64_t x, int64_t y) const;

		/**
		* return - returns the index which can be used in SetTile(x, y, idx)
		*/
		int AddSprite(const Sprite &sprite);
	};
}