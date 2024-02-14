#pragma once

#include <ecs/component.h>
#include "transform.h"
#include <box2d/box2d.h>

namespace engine { namespace physics {
	class RigidBody2D : public Component {
		b2Body *body;
		b2Fixture *fixture;
		Transform *transform = nullptr;
		b2Vec2 velocity;
		float drag;
		b2BodyType type;
		b2MassData massData;
		bool shouldUpdate = false;
	public:
		RigidBody2D(b2BodyType type = b2BodyType::b2_dynamicBody): type(type), Component() {}
		~RigidBody2D();

		void Awake() override;
		void Start() override;
		void FixedUpdate() override;
		void LateUpdate() override;

		inline b2BodyType GetType() const { return type; }
		void SetType(b2BodyType newType) { type = newType; shouldUpdate = true; }

		inline const Transform &GetTransform() const { return *transform; }
		void SetTransform(const Transform &transform);
		void SetTransform(const b2Transform &transform);

		inline Transform::Position GetPos() const { return transform->GetPos(); }

		inline const b2Vec2 &GetVelocity() const { return velocity; }
		void SetVelocity(const b2Vec2 &vel);
		void AddVelocity(const b2Vec2 &vel);

		inline float GetDrag() const { return drag; }
		void SetDrag(float drag);

		inline float GetMass() const { return massData.mass; }
		void SetMass(float mass);

		inline b2Body &GetBody() { return *body; }
	};
}}