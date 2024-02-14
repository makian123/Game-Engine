#include <ecs/gameobject.h>
#include <ecs/scene.h>
#include <stack>

namespace engine {
	void GameObject::Awake() {
		for (auto &pair : components) {
			for (auto &component : pair.second) {
				component->Awake();
			}
		}
		for (auto &child : children) {
			child->Awake();
		}
	}
	void GameObject::Start() {
		if (!enabled) return;
		toInitialize.clear();

		for (auto& pair : components) {
			for (auto& component : pair.second) {
				component->Start();
			}
		}
		for (auto &child : children) {
			child->Start();
		}
	}
	void GameObject::FixedUpdate() {
		if (!enabled) return;

		while (toInitialize.size()) {
			auto &comp = toInitialize.back();
			comp->Start();

			toInitialize.pop_back();
		}

		for (auto& pair : components) {
			for (auto& component : pair.second) {
				component->FixedUpdate();
			}
		}
		for (auto &child : children) {
			child->FixedUpdate();
		}
	}
	void GameObject::Update() {
		if (!enabled) return;

		while (toInitialize.size()) {
			auto &comp = toInitialize.back();
			comp->Start();

			toInitialize.pop_back();
		}

		for (auto &pair : components) {
			for (auto &component : pair.second) {
				component->Update();
			}
		}
		for (auto &child : children) {
			child->Update();
		}
	}
	void GameObject::LateUpdate() {
		if (!enabled) return;

		for (auto &pair : components) {
			for (auto &component : pair.second) {
				component->LateUpdate();
			}
		}
		for (auto &child : children) {
			child->LateUpdate();
		}
	}

	void GameObject::OnCollisionEnter2D(const physics::Collider2D *collision) {
		for (auto &pair : components) {
			for (auto &component : pair.second) {
				component->OnCollisionEnter2D(collision);
			}
		}
	}
	void GameObject::OnCollisionExit2D(const physics::Collider2D *collision) {
		for (auto &pair : components) {
			for (auto &component : pair.second) {
				component->OnCollisionExit2D(collision);
			}
		}
	}
	void GameObject::OnTriggerEnter2D(const physics::Collider2D *collision) {
		for (auto &pair : components) {
			for (auto &component : pair.second) {
				component->OnTriggerEnter2D(collision);
			}
		}
	}
	void GameObject::OnTriggerExit2D(const physics::Collider2D *collision) {
		for (auto &pair : components) {
			for (auto &component : pair.second) {
				component->OnTriggerExit2D(collision);
			}
		}
	}

	GameObject* GameObject::FindChild(const std::string& name) {
		std::stack<GameObject*> toSearch;

		for (auto child : children) {
			toSearch.push(child);
		}
		while (toSearch.size()) {
			GameObject* top = toSearch.top();
			toSearch.pop();

			if (top->name == name) return top;

			for (auto child : top->children) {
				toSearch.push(child);
			}
		}

		return nullptr;
	}

	b2World* GameObject::GetWorld() {
		return parentScene->GetWorld();
	}
}