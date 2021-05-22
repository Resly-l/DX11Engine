#pragma once
#include "component.h"

class PhysicsComponent : public Component<PhysicsComponent>
{
	friend class PhysicsSystem;

private:
	// imgui variables
	static std::vector<std::string> boundingVolumeNames;
	inline static size_t boundingVolumeIndex = 0;
	size_t colliderIndex = -1;

	std::unique_ptr<BoundingVolume> pCollider;

	float mass = 1.0f;
	float inverseMass = 1.0f;
	float friction = 0.2f;
	float restitution = 0.1f;
	float gravityFactor = 1.0f;

	Vector velocity;
	Vector force;

public:
	BoundingVolume* GetCollider() const { return pCollider.get(); }

	void Integrate(double deltaSeconds);

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	void DrawColliderCombo();
	void DrawColliderButton();
	void DrawColliderWidget();
};