#pragma once

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <SDL/SDL.h>

#include <ecs/scene.h>
#include <inputManager/inputManager.h>
#include <graphics/shader.h>

namespace engine {
	namespace graphics{
	class Window {
	private:
		std::string title;
		std::thread renderThread;
		size_t width, height;
		SDL_Window *sdlWindow = nullptr;
		SDL_GLContext context = nullptr;
		InputManager &input;
		Shader *shader;
		Scene *currScene;
		std::atomic_bool shouldExit;
	public:
		explicit Window(const std::string &title, size_t width, size_t height, InputManager &input_);
		~Window();

		void Update();

		void Open();
		void Close();

		bool ShouldExit() const { return shouldExit.load(); }

		const SDL_Window *GetSDLWindow() const { return sdlWindow; }
		const SDL_GLContext &GetContext() const { return context; }

		void SetScene(Scene *scene_) { currScene = scene_; }
		Scene *GetScene() { return currScene; }
	};
}}