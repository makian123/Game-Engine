#pragma once

#include <map>
#include <SDL/SDL_keycode.h>
#include <glm/vec2.hpp>

namespace engine {
	namespace graphics {
		class Window;
	}

	class InputManager {
	private:
		static constexpr int keyCount = 1024;
		std::map<SDL_Keycode, bool> keys, lastKeys;
		std::map<int, bool> buttons, lastButtons;
		glm::vec2 mousePos, mouseMove;

		void PressKey(SDL_Keycode key);
		void ReleaseKey(SDL_Keycode key);
		void PressMouse(int btn);
		void ReleaseMouse(int btn);

		void SetMousePos(const glm::ivec2 &pos);
		void SetMouseMove(const glm::ivec2& pos);

		void Update();
	public:
		friend class graphics::Window;
		InputManager();

		bool GetKey(SDL_KeyCode key) const;
		bool GetKeyDown(SDL_KeyCode key) const;

		glm::vec2 GetMousePos() const;
		glm::vec2 GetMouseMovement() const;

		bool GetMouseButton(int button) const;
		bool GetMouseButtonDown(int button) const;
	};
}