#include "graphics/window.h"

#include <graphics/2D/sprite.h>
#include <graphics/3D/mesh.h>
#include <graphics/camera.h>
#include <ecs/scene.h>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include <SDL/SDL_opengl.h>

namespace engine::graphics{
	std::shared_ptr<Texture> tx;

	Window::Window(const std::string &title_, size_t width_, size_t height_, InputManager &input_)
		:title(title_), width(width_), height(height_), input(input_), shouldExit(true) {
	}
	Window::~Window() {
		if (sdlWindow) SDL_DestroyWindow(sdlWindow);
	}


	void Window::Update() {
		currScene->Start();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		input.Update();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					shouldExit = true;
					break;
				case SDL_KEYDOWN:
					input.PressKey(event.key.keysym.sym);
					break;
				case SDL_KEYUP:
					input.ReleaseKey(event.key.keysym.sym);
					break;
				case SDL_MOUSEMOTION:
					input.SetMousePos(glm::ivec2(event.motion.x, event.motion.y));
					input.SetMouseMove(glm::ivec2(event.motion.xrel, event.motion.yrel));
					break;
				case SDL_MOUSEBUTTONDOWN:
					input.PressMouse(event.button.button);
					break;
				case SDL_MOUSEBUTTONUP:
					input.ReleaseMouse(event.button.button);
					break;
			}
		}

		auto cam = const_cast<Camera*>(currScene->GetCamera());

		currScene->Update();
		currScene->Draw();

		SDL_GL_SwapWindow(sdlWindow);
	}

	void Window::Open() {
		sdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

		if(currScene)
			currScene->Start();

		context = SDL_GL_CreateContext(sdlWindow);
		SDL_GL_MakeCurrent(sdlWindow, context);

		shouldExit = false;
	}
	void Window::Close() {
		if (shouldExit) return;

		shouldExit = true;
		if (sdlWindow) SDL_DestroyWindow(sdlWindow);
		sdlWindow = nullptr;
	}
}