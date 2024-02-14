#include <physics/circleCollider2d.h>
#include <physics/rigidbody2d.h>
#include <ecs/gameobject.h>
#include <stdexcept>
#include <iostream>

namespace engine { namespace physics {
	CircleCollider2D::CircleCollider2D(float radius_, bool isTrigger): radius(radius_), Component(), Collider2D(isTrigger) {}
	CircleCollider2D::~CircleCollider2D() {
	}

	void CircleCollider2D::Start() {
		object = gameobject;
		attachedRigidbody = gameobject->GetComponent<RigidBody2D>();
		if (!attachedRigidbody) {
			throw std::exception("No rigid body found");
		}

		b2BodyUserData data;
		data.pointer = reinterpret_cast<uintptr_t>(dynamic_cast<Collider2D *>(this));
		attachedRigidbody->GetBody().GetUserData() = data;

		b2CircleShape circle;
		circle.m_p.Set(gameobject->GetTransform().GetPos().x, gameobject->GetTransform().GetPos().y);
		circle.m_radius = radius;
		fixture = attachedRigidbody->GetBody().CreateFixture(&circle, 1.0f);

		if (triggerCollider) {
			fixture->SetSensor(true);
		}
	}
	void CircleCollider2D::Update() {
		auto contact = fixture->GetBody()->GetContactList();
		std::unordered_set<Collider2D *> colliders;
		while (contact) {
			auto currContact = contact->contact;
			if (currContact->IsTouching()) {
				Collider2D *otherBodies[2] = {
					reinterpret_cast<Collider2D *>(currContact->GetFixtureB()->GetBody()->GetUserData().pointer),
					reinterpret_cast<Collider2D *>(currContact->GetFixtureA()->GetBody()->GetUserData().pointer)
				};

				for (int i = 0; i < 2; ++i) {
					if (otherBodies[i]->object->GetID() == gameobject->GetID()) continue;

					colliders.insert(otherBodies[i]);

					if (!currentCollisions.contains(otherBodies[i])) {
						currentCollisions.insert(otherBodies[i]);
						if (!triggerCollider) {
							gameobject->OnTriggerEnter2D(otherBodies[i]);
						}
						else {
							gameobject->OnCollisionEnter2D(otherBodies[i]);
						}
					}
				}
			}

			contact = contact->next;
		}

		for (auto it = currentCollisions.cbegin(); it != currentCollisions.cend();) {
			auto collider = *it;

			if (!colliders.contains(collider)) {
				gameobject->OnCollisionExit2D(collider);
				currentCollisions.erase(it++);
			}
			else {
				++it;
			}
		}
	}
}}