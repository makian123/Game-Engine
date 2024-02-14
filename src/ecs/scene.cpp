#include <ecs/scene.h>
#include <stack>

#include <graphics/drawable.h>
#include <graphics/pointLight.h>
#include <ecs/gameobject.h>
#include <ecs/component.h>

namespace engine {
	Scene::Scene() : running(false) {
		world = new b2World(b2Vec2_zero);
		//world = new b2World(b2Vec2(0.0f, -9.81f));
	}
	Scene::~Scene() {
		running = false;

		for (auto &child : objects) {
			delete child;
		}

		delete world;
	}

	GameObject *Scene::FindGameObject(const std::string &name) {
		for (auto child : objects) {
			if (child->name == name) return child;

			GameObject *found = child->FindChild(name);
			if (found) return found;
		}

		return nullptr;
	}

	GameObject *Scene::Instantiate(GameObject *obj, GameObject *parent) {
		if (!obj) return nullptr;

		obj->parent = parent;
		obj->parentScene = this;

		if (parent) {
			parent->children.push_back(obj);
			return obj;
		}

		objects.push_back(obj);
		return obj;
	}
	void Scene::Destroy(GameObject *obj) {
		if (!obj) return;

		if (obj->parent) {
			obj->parent->children.erase(std::find(obj->parent->children.begin(), obj->parent->children.end(), obj));
		}
		objects.erase(std::find(objects.begin(), objects.end(), obj));
		delete obj;
	}

	void Scene::SetCamera(graphics::Camera *cam) {
		camera = cam;
	}

	void Scene::AddLight(Component *light) {
		lights.push_back(light);
	}
	void Scene::RemoveLight(Component *light){
		lights.erase(std::find(lights.begin(), lights.end(), light));
	}

	void Scene::Update() {
		world->Step(1.0f, 6, 2);

		for (auto &object : objects) {
			object->FixedUpdate();
		}
		for (auto &object : objects) {
			object->Update();
		}

		for (auto &object : objects) {
			auto renderer = object->GetRenderer();
			if (renderer)
				renderer->Draw(*camera);
		}

		for (auto &object : objects) {
			object->LateUpdate();
		}
	}
	void Scene::Start() {
		if (running) return;

		for (auto &object : objects) {
			object->Awake();
		}
		for (auto &object : objects) {
			object->Start();
		}

		running = true;
	}

	void Scene::Draw() {
		std::stack<GameObject *> toDraw;

		for (auto &object : objects) {
			toDraw.push(object);
		}

		if (toDraw.size()) {
			camera->Use();
		}
		while (toDraw.size() && camera) {
			auto currObj = toDraw.top();
			toDraw.pop();

			for (auto &object : currObj->children) {
				toDraw.push(object);
			}

			if (currObj->renderer)
				currObj->renderer->Draw(*camera);
		}
	}
}