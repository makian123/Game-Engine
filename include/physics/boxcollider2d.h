#pragma once

#include <box2d/box2d.h>
#include <ecs/component.h>
#include "collider2d.h"

namespace engine { namespace physics {
	class BoxCollider2D: public Component, public Collider2D {
		b2Fixture *fixture;
		float width, height;

	public:
		BoxCollider2D(float width, float height, bool isTrigger = false);
		~BoxCollider2D();

		void Start() override;
		void Update() override;
	};
}}