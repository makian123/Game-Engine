#pragma once

#include "collider2d.h"
#include <ecs/component.h>

namespace engine { namespace physics {
	class CircleCollider2D : public Component, public Collider2D {
		float radius;

		public:
		CircleCollider2D(float radius, bool isTrigger = false);
		~CircleCollider2D();

		void Start() override;
		void Update() override;
	};
}}