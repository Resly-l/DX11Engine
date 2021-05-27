#pragma once
#include "system.h"

class PhysicsComponent;

class PhysicsSystem : public System<PhysicsSystem>
{
private:
	struct Contact
	{
		Vector normal;
		float depth;
	};

private:
	float gravity = 9.0f;

	// simulate air resistance
	float linearDampling = 0.0015f;

	// calculation error occurs due to floating point error
	// im using simple correction method called linear projection
	// it simply pushes bodies along contact normal by percentage
	float linearProjection = 0.8f; // usally 0.2 ~ 0.8
	float correctionThreshold = 0.01f; // usally 0.01 ~ 0.02

public:
	void Update(double deltaSeconds, bool simulate);

	void DrawWidget() override;

private:
	void ApplyNaturalForces(PhysicsComponent* pPhysicsComponent);
	void HandleCollision(PhysicsComponent* pLhs, PhysicsComponent* pRhs, const Contact& contact);

	std::optional<Contact> CheckCollision(PhysicsComponent* pLhs, PhysicsComponent* pRhs);
	std::optional<Contact> CollideBoxToBox(PhysicsComponent* pLhs, PhysicsComponent* pRhs);
	std::optional<Contact> CollideBoxToSphere(PhysicsComponent* pLhs, PhysicsComponent* pRhs);
	std::optional<Contact> CollideSphereToSphere(PhysicsComponent* pLhs, PhysicsComponent* pRhs);
};