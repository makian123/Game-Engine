#pragma once

#include <unordered_set>
#include <box2d/box2d.h>

namespace engine { 
	class GameObject;

	namespace physics {
	class RigidBody2D;

	class Collider2D {
		protected:
		class Listener : b2ContactListener {
			friend class Collider2D;
		};

		protected:
		float density{ 0.0f };
		b2Fixture *fixture = nullptr ;
		RigidBody2D *attachedRigidbody = nullptr ;
		std::unordered_set<Collider2D *> currentCollisions;
		bool triggerCollider = false;

		public:
		GameObject *object = nullptr;

		Collider2D(bool trigger = false) : triggerCollider(trigger) {}
		virtual ~Collider2D() {}
	};
}}