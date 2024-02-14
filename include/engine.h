#pragma once

#include <string>
#include <timer.h>
#include <ecs/scene.h>
#include <graphics/window.h>
#include <inputManager/inputManager.h>

namespace engine {
	void Start(const std::string &windName, size_t width, size_t height);
	void Update();
	void Stop();

	graphics::Window &GetWindowInstance();
	extern InputManager input;
	extern Time timer;

	void SetScene(Scene *scene);
	Scene *GetScene();
	
}