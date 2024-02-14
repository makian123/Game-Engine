#include <physics/rigidbody2d.h>
#include <ecs/gameobject.h>
#include <ecs/scene.h>

namespace engine { namespace physics {
	RigidBody2D::~RigidBody2D() {
		gameobject->GetWorld()->DestroyBody(body);
		body = nullptr;
	}

	void RigidBody2D::Awake() {
		b2BodyDef def;
		def.type = type;
		def.position = b2Vec2_zero;

		body = gameobject->GetWorld()->CreateBody(&def);
		transform = (engine::physics::Transform *)&gameobject->GetTransform();
		body->GetMassData(&massData);
	}
	void RigidBody2D::Start() {
		body->SetTransform(b2Vec2{ transform->GetPos().x, transform->GetPos().y }, 0.0f);
	}
	void RigidBody2D::FixedUpdate() {
		if (gameobject->IsEnabled() != body->IsEnabled()) { body->SetEnabled(gameobject->IsEnabled()); }
		if (!gameobject->IsEnabled()) return;

		transform->SetPos(body->GetTransform().p.x, body->GetTransform().p.y, 0.0f);
		velocity = body->GetLinearVelocity();
	}
	void RigidBody2D::LateUpdate() {
		if (shouldUpdate) {
			body->SetTransform(b2Vec2{ transform->GetPos().x, transform->GetPos().y }, 0.0f);
			body->SetLinearDamping(drag);
			body->SetLinearVelocity(velocity);
			body->SetType(type);

			shouldUpdate = false;
		}
	}

	void RigidBody2D::SetTransform(const Transform &transform_) {
		*transform = transform_;

		body->SetTransform(b2Vec2{ transform->GetPos().x, transform->GetPos().y }, 0.0f);
		//shouldUpdate = true;
	}
	void RigidBody2D::SetTransform(const b2Transform &transform_) {
		*transform = Transform(transform_);
		body->SetTransform(b2Vec2{ transform->GetPos().x, transform->GetPos().y }, 0.0f);
		//shouldUpdate = true;
	}

	void RigidBody2D::SetVelocity(const b2Vec2 &vel) {
		velocity = vel;
		body->SetLinearVelocity(velocity);
		//shouldUpdate = true;
	}
	void RigidBody2D::AddVelocity(const b2Vec2 &vel) {
		velocity += vel;
		body->SetLinearVelocity(velocity);
		//shouldUpdate = true;
	}
	
	void RigidBody2D::SetDrag(float drag_) {
		drag = drag_;
		body->SetLinearDamping(drag);
		//shouldUpdate = true;
	}
	void RigidBody2D::SetMass(float mass_) {
		massData.mass = mass_;
		body->SetMassData(&massData);
		//shouldUpdate = true;
	}
}}