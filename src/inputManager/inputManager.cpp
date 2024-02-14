#include "inputManager/inputManager.h"
#include <iostream>
#include <SDL/SDL_mouse.h>

namespace engine {
	InputManager::InputManager() {
		for (int i = 0; i < keyCount; ++i) {
			keys[static_cast<SDL_Keycode>(i)] = false;
			lastKeys[static_cast<SDL_Keycode>(i)] = false;
		}

		for (int i = 1; i < 6; ++i) {
			buttons[i] = false;
		}
	}

	void InputManager::Update() {
		for (auto &[key, val] : keys) {
			lastKeys[key] = val;
		}
		for (auto &[btn, val] : buttons) {
			lastButtons[btn] = val;
		}

		SetMouseMove({ 0, 0 });
	}

	void InputManager::SetMousePos(const glm::ivec2 &pos) {
		mousePos = pos;
	}
	void InputManager::SetMouseMove(const glm::ivec2& pos) {
		mouseMove = glm::vec2(pos.x / 255.0f, pos.y / 255.0f);
	}
	void InputManager::PressKey(SDL_Keycode key) {
		keys[key] = true;
	}
	void InputManager::ReleaseKey(SDL_Keycode key) {
		keys[key] = false;
	}
	void InputManager::PressMouse(int btn) {
		buttons[btn] = true;
	}
	void InputManager::ReleaseMouse(int btn){
		buttons[btn] = false;
	}

	bool InputManager::GetKey(SDL_KeyCode key) const {
		if (key < 0 || key > keyCount) return false;

		return keys.at(key);
	}
	bool InputManager::GetKeyDown(SDL_KeyCode key) const {
		if (key < 0 || key > keyCount) return false;

		return (keys.at(key) && !lastKeys.at(key));
	}
	
	glm::vec2 InputManager::GetMousePos() const {
		return mousePos;
	}
	glm::vec2 InputManager::GetMouseMovement() const {
		return mouseMove;
	}
	bool InputManager::GetMouseButton(int button) const {
		if (button < 0 || !buttons.contains(button)) return false;

		return buttons.at(button);
	}
	bool InputManager::GetMouseButtonDown(int button) const {
		if (button < 0 || !buttons.contains(button)) return false;

		return (buttons.at(button) && !lastButtons.at(button));
	}
}