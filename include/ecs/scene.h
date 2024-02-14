#pragma once

#include <graphics/camera.h>
#include <box2d/box2d.h>

namespace engine {
	class GameObject;
	class Component;

	class Scene {
	private:
		std::vector<GameObject*> objects;
		std::vector<Component *> lights;
		graphics::Camera *camera;
		b2World* world;
		bool running;

	public:
		Scene();
		~Scene();

		GameObject* FindGameObject(const std::string& name);
		GameObject* Instantiate(GameObject *obj, GameObject* parent = nullptr);
		void Destroy(GameObject *obj);

		void Update();
		void Start();
		void Draw();

		void SetCamera(graphics::Camera *cam);
		inline const graphics::Camera *GetCamera() const { return camera; }

		void AddLight(Component *light);
		void RemoveLight(Component *light);
		inline const std::vector<Component *> &GetLights() const { return lights; }

		b2World* GetWorld() { return world; }

		inline bool IsRunning() const { return running; }
	};
}