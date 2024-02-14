#pragma once

#include <new>
#include <memory>
#include <vector>
#include <cassert>
#include <iostream>
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <engine.h>
#include <box2d/box2d.h>
#include <physics/transform.h>
#include <physics/collider2d.h>
#include <utils/uuidgenerator.h>
#include <graphics/drawable.h>

#include "component.h"

namespace engine {
	class Scene;
}

namespace engine {

	class GameObject final {
	private:
		physics::Transform transform;
		
		std::unordered_map<int, std::vector<Component *>> componentScript;
		std::unordered_map<std::type_index, std::vector<Component *>> components;
		std::vector<Component *> toInitialize;
		graphics::Drawable *renderer;
		GameObject* parent;
		Scene* parentScene;
		std::vector<GameObject*> children;
		bool enabled = true;
		UUID uuid;

	public:
		std::string name{};

		GameObject(const std::string &name_ = "GameObject")
			:name(name_), uuid(), parent(nullptr), parentScene(nullptr) {}
		friend class Scene;

		~GameObject() {
			for (auto &pair : components) {
				for (auto &component : pair.second) {
					component->OnDestroy();
				}
			}

			for (auto child : children) {
				delete child;
			}
		}

		///Adds the specified component
		template<DerivedComponent T, typename ...Args>
		T &AddComponent(Args&& ...args) {
			auto component = new T(args...);
			std::type_index index = std::type_index(typeid(T));

			components[index].push_back(component);
			component->gameobject = this;
			toInitialize.push_back(component);

			if constexpr (std::is_base_of_v<graphics::Drawable, T>) {
				renderer = component;
			}

			component->Awake();

			return *component;
		}
		template<DerivedComponent T>
		void RemoveComponent(T *comp) {
			assert(comp);

			std::type_index index = std::type_index(typeid(T));
			if (!components.contains(index)) return;
			auto location = std::find(components[index].begin(), components[index].end(), comp);
			if (location == components[index].end()) return;

			delete *location;
			components[index].erase(location);
		}
		template<DerivedComponent T>
		bool HasComponent() const {
			return components.contains(std::type_index(typeid(T)));
		}
		
		///Returns the first component found in the gameobject
		template<DerivedComponent T>
		T *GetComponent() {
			auto index = std::type_index(typeid(T));
			if (!components.contains(index)) return nullptr;

			return dynamic_cast<T*>(components[index].front());
		}
		template<DerivedComponent T>
		std::vector<T*> GetComponents() {
			std::vector<T*> vec;
			auto index = std::type_index(typeid(T));
			if(!components.contains(index))
				return vec;

			for (auto& component : components[index]) {
				vec.push_back(dynamic_cast<T*>(component));
			}

			return vec;
		}

		static GameObject *Factory() {
			return new GameObject();
		}
		void AddRef() {

		}
		void Release() {

		}

		GameObject* FindChild(const std::string& name);

		inline bool IsEnabled() const { return enabled; }
		inline const physics::Transform &GetTransform() const { return transform; }
		b2World* GetWorld();
		inline const Scene *GetScene() const { return parentScene; }
		inline UUID GetID() const { return uuid; }
		inline const graphics::Drawable *GetRenderer() const { return renderer; }

		void SetTransform(const physics::Transform &transform_) { transform = transform_; }
		void SetPos(const physics::Transform::Position &pos_) { transform.SetPos(pos_); }
		void SetEnabled(bool enabled_) { enabled = enabled_; }

		void Awake();
		void Start();
		void FixedUpdate();
		void Update();
		void LateUpdate();

		void OnCollisionEnter2D(const physics::Collider2D *collision);
		void OnCollisionExit2D(const physics::Collider2D *collision);

		void OnTriggerEnter2D(const physics::Collider2D *collision);
		void OnTriggerExit2D(const physics::Collider2D *collision);
	};
}