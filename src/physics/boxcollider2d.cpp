#include <physics/boxcollider2d.h>
#include <physics/rigidbody2d.h>
#include <ecs/gameobject.h>
#include <stdexcept>
#include <iostream>

namespace engine { namespace physics {
	BoxCollider2D::BoxCollider2D(float width_, float height_, bool isTrigger): width(width_), height(height_), Component(), Collider2D(isTrigger) {}
	BoxCollider2D::~BoxCollider2D() {
	}

	void BoxCollider2D::Start() {
		object = gameobject;
		attachedRigidbody = gameobject->GetComponent<RigidBody2D>();
		if (!attachedRigidbody) {
			throw std::exception("No rigid body found");
		}

		b2BodyUserData data;
		data.pointer = reinterpret_cast<uintptr_t>(dynamic_cast<Collider2D*>(this));
		attachedRigidbody->GetBody().GetUserData() = data;

		b2PolygonShape groundBox;
		groundBox.SetAsBox(width / 2.0f, height / 2.0f);
		fixture = attachedRigidbody->GetBody().CreateFixture(&groundBox, 1.0f);

		if (triggerCollider) {
			fixture->SetSensor(true);
		}
	}
	void BoxCollider2D::Update() {
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
						if (triggerCollider) gameobject->OnTriggerEnter2D(otherBodies[i]);
						else gameobject->OnCollisionEnter2D(otherBodies[i]);
					}
				}
			}

			contact = contact->next;
		}

		for (auto it = currentCollisions.cbegin(); it != currentCollisions.cend();) {
			auto collider = *it;

			if (!colliders.contains(collider)) {
				if (triggerCollider) {
					gameobject->OnTriggerExit2D(collider);
				}
				else {
					gameobject->OnCollisionExit2D(collider);
				}
				currentCollisions.erase(it++);
			}
			else {
				++it;
			}
		}
	}
}}