#include "engine.h"
#include <GL/glew.h>
#include <STB/stb_image.h>
#include <SDL/SDL_mixer.h>

#include <iostream>

namespace engine {
	static graphics::Window *mainWind;

	InputManager input;
	Time timer;

	void SetScene(Scene *scene) {
		if (!mainWind || !scene) return;

		mainWind->SetScene(scene);
		scene->Start();
	}
	Scene *GetScene() {
		if (!mainWind) return nullptr;

		return mainWind->GetScene();
	}

	void Start(const std::string &windName, size_t width, size_t height) {
		stbi_set_flip_vertically_on_load(true);
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_ShowCursor(SDL_ENABLE);

		mainWind = new graphics::Window(windName, width, height, input);
		mainWind->Open();

		glewInit();
		SDL_GL_SetSwapInterval(1);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glViewport(0, 0, width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_TEXTURE_2D);

		char *glVer = (char *)glGetString(GL_VERSION);
		//std::cout << "GL: " << glVer << "\n";
		if (Mix_OpenAudio(44800, AUDIO_S32SYS, 2, 1024) < 0) {
			std::cerr << "Unable to open audio: " << SDL_GetError() << "\n";
			exit(-1);
		}
		if (Mix_AllocateChannels(8) < 0) {
			std::cerr << "Unable to allocate channels: " << SDL_GetError() << "\n";
			exit(-1);
		}
	}
	void Update() {
		timer.Begin();
		if (mainWind) mainWind->Update();
		timer.End();
	}
	void Stop() {
		mainWind->Close();

		delete mainWind->GetScene();
		delete mainWind;

		SDL_Quit();
	}

	graphics::Window &GetWindowInstance() {
		return static_cast<graphics::Window&>(*mainWind);
	}
}