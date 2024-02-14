#pragma once

#include <string>
#include <cstdint>
#include <concepts>
#include <physics/collider2d.h>
#include <utils/uuidgenerator.h>

namespace engine {
	class GameObject;
	class Scene;

	class Component {
	protected:
		UUID uuid;
		GameObject* gameobject;

	protected:
		Component() : uuid(), gameobject(nullptr) {}
		
	public:
		friend class Scene;
		friend class GameObject;
		virtual ~Component() {
			OnDestroy();
		}

		GameObject *GameObject() { return gameobject; }

		/// Called before creation
		virtual void Awake() {}
		/// Called on the creation of the object
		virtual void Start() {}
		/// Called before update
		virtual void FixedUpdate() {}
		/// Called every frame
		virtual void Update() {}
		/// Called after update
		virtual void LateUpdate() {}
		/// Called when object destroyed
		virtual void OnDestroy() {}
		/// Called once on 2D collision enter
		virtual void OnCollisionEnter2D(const physics::Collider2D *collision) {}
		/// Called once on 2D collision exit
		virtual void OnCollisionExit2D(const physics::Collider2D *collision) {}
		/// Called once on 2D trigger enter
		virtual void OnTriggerEnter2D(const physics::Collider2D *collision) {}
		/// Called once on 2D trigger exit
		virtual void OnTriggerExit2D(const physics::Collider2D *collision) {}

		inline UUID GetID() const { return uuid; }
		inline engine::GameObject *GetGameObject() const { return gameobject; }
	};
	

	template<typename T>
	concept DerivedComponent = std::is_base_of_v<Component, T>;
}