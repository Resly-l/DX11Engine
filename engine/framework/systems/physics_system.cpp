#include "physics_system.h"
#include "components/physics_component.h"
#include "components/transform_component.h"
#include "entity.h"
#include "component_factory.h"

void PhysicsSystem::Update(double deltaSeconds, bool simulate)
{
	if (simulate == false)
	{
		return;
	}

	auto& componentPtrs = ComponentFactory::GetInstances(PhysicsComponent::ID);

	for (size_t i = 0; i < componentPtrs.size(); i++)
	{
		auto pLhs = static_cast<PhysicsComponent*>(componentPtrs[i].get());

		if (pLhs->GetOwner()->GetComponent<TransformComponent>() == nullptr)
		{
			continue;
		}

		if (pLhs->mass != 0.0f)
		{
			ApplyNaturalForces(pLhs);
			pLhs->Integrate(deltaSeconds);
		}

		for (size_t j = i + 1; j < componentPtrs.size(); j++)
		{
			auto pRhs = static_cast<PhysicsComponent*>(componentPtrs[j].get());

			if (pRhs->GetOwner()->GetComponent<TransformComponent>() == nullptr)
			{
				continue;
			}

			if (auto contact = CheckCollision(pLhs, pRhs))
			{
				HandleCollision(pLhs, pRhs, *contact);
			}
		}
	}

}

void PhysicsSystem::DrawWidget()
{
	ImGui::DragFloat("gravity", &gravity, 0.1f);
	ImGui::DragFloat("linear dampling", &linearDampling, 0.001f);
}

void PhysicsSystem::ApplyNaturalForces(PhysicsComponent* pPhysicsComponent)
{
	pPhysicsComponent->force += Vector(0.0f, -gravity * pPhysicsComponent->gravityFactor, 0.0f, 0.0f) - pPhysicsComponent->velocity * linearDampling;
}

void PhysicsSystem::HandleCollision(PhysicsComponent* pLhs, PhysicsComponent* pRhs, const Contact& contact)
{
	Vector vRelativeVelocity = pRhs->velocity - pLhs->velocity;
	const float fVelocityAlongNormal = vRelativeVelocity.Dot3(contact.normal);

	// if two bodies are already separating don't resolve collision
	if (fVelocityAlongNormal > 0.0f)
	{
		return;
	}

	const float fSumInverseMass = pLhs->inverseMass + pRhs->inverseMass;
	const float restitution = std::min(pLhs->restitution, pRhs->restitution);
	const float fImpulseMagnitude = -(1.0f + restitution) * fVelocityAlongNormal / (fSumInverseMass);
	const Vector vImpulse = contact.normal * fImpulseMagnitude;
	const Vector positionCorrection = contact.normal * std::max(contact.depth - correctionThreshold, 0.0f) / fSumInverseMass * linearProjection;

	// apply impulse & correction
	if (pLhs->mass != 0.0f)
	{
		pLhs->velocity -= vImpulse * pLhs->inverseMass;

		auto pTransformComponent = pLhs->GetOwner()->GetComponent<TransformComponent>();
		pTransformComponent->SetAbsolutePosition(pTransformComponent->GetAbsoluteTransform().Decompose().position - positionCorrection * pLhs->inverseMass);
	}
	if (pRhs->mass != 0.0f)
	{
		pRhs->velocity += vImpulse * pRhs->inverseMass;

		auto pTransformComponent = pRhs->GetOwner()->GetComponent<TransformComponent>();
		pTransformComponent->SetAbsolutePosition(pTransformComponent->GetAbsoluteTransform().Decompose().position + positionCorrection * pRhs->inverseMass);
	}

	vRelativeVelocity = pRhs->velocity - pLhs->velocity;
	const Vector vTangent = (vRelativeVelocity - contact.normal * (vRelativeVelocity.Dot3(contact.normal))).GetNormalized3();
	const float frictionMagnitude = -vTangent.Dot3(vRelativeVelocity) / fSumInverseMass;
	const float fSumFrictionSquared = pLhs->friction * pLhs->friction + pRhs->friction * pRhs->friction;
	const Vector vFriction = abs(frictionMagnitude) < fImpulseMagnitude * fSumFrictionSquared ? vTangent * frictionMagnitude : vTangent * -fImpulseMagnitude * fSumFrictionSquared;

	if (pLhs->mass != 0.0f)
	{
		pLhs->velocity -= vFriction * pLhs->inverseMass;
	}
	if (pRhs->mass != 0.0f)
	{
		pRhs->velocity += vFriction * pRhs->inverseMass;
	}
}

std::optional<PhysicsSystem::Contact> PhysicsSystem::CheckCollision(PhysicsComponent* pLhs, PhysicsComponent* pRhs)
{
	// lhs == box
	if (dynamic_cast<BoundingBox*>(pLhs->GetCollider()))
	{
		// rhs == box
		if (dynamic_cast<BoundingBox*>(pRhs->GetCollider()))
		{
			return CollideBoxToBox(pLhs, pRhs);
		}
		// rhs == sphere
		else if (dynamic_cast<BoundingSphere*>(pRhs->GetCollider()))
		{
			return CollideBoxToSphere(pLhs, pRhs);
		}
	}
	// lhs == sphere
	else if (dynamic_cast<BoundingSphere*>(pLhs->GetCollider()))
	{
		// rhs == box
		if (dynamic_cast<BoundingBox*>(pRhs->GetCollider()))
		{
			auto contact = CollideBoxToSphere(pRhs, pLhs);
			contact->normal *= -1.0f;
			return contact;
		}
		// rhs == sphere
		else if (dynamic_cast<BoundingSphere*>(pRhs->GetCollider()))
		{
			return CollideSphereToSphere(pLhs, pRhs);
		}
	}

	return std::nullopt;
}

std::optional<PhysicsSystem::Contact> PhysicsSystem::CollideBoxToBox(PhysicsComponent* pLhs, PhysicsComponent* pRhs)
{
	BoundingBox lhsBox = *static_cast<BoundingBox*>(pLhs->GetCollider());
	BoundingBox rhsBox = *static_cast<BoundingBox*>(pRhs->GetCollider());

	const Matrix lhsTransform = pLhs->GetOwner()->GetComponent<TransformComponent>()->GetAbsoluteTransform();
	const Matrix rhsTransform = pRhs->GetOwner()->GetComponent<TransformComponent>()->GetAbsoluteTransform();

	lhsBox.minExtent *= lhsTransform;
	lhsBox.maxExtent *= lhsTransform;

	rhsBox.minExtent *= rhsTransform;
	rhsBox.maxExtent *= rhsTransform;

	// if not colliding
	if (lhsBox.minExtent.x > rhsBox.maxExtent.x || rhsBox.minExtent.x > lhsBox.maxExtent.x ||
		lhsBox.minExtent.y > rhsBox.maxExtent.y || rhsBox.minExtent.y > lhsBox.maxExtent.y ||
		lhsBox.minExtent.z > rhsBox.maxExtent.z || rhsBox.minExtent.z > lhsBox.maxExtent.z)
	{
		return std::nullopt;
	}

	std::pair<float, float> xPenetration = { lhsBox.minExtent.x - rhsBox.maxExtent.x, rhsBox.minExtent.x - lhsBox.maxExtent.x };
	std::pair<float, float> yPenetration = { lhsBox.minExtent.y - rhsBox.maxExtent.y, rhsBox.minExtent.y - lhsBox.maxExtent.y };
	std::pair<float, float> zPenetration = { lhsBox.minExtent.z - rhsBox.maxExtent.z, rhsBox.minExtent.z - lhsBox.maxExtent.z };

	float minX = std::min(abs(xPenetration.first), abs(xPenetration.second));
	float minY = std::min(abs(yPenetration.first), abs(yPenetration.second));
	float minZ = std::min(abs(zPenetration.first), abs(zPenetration.second));

	Contact contact;

	// x axis is minimal penetration
	if (minX < minY && minX < minZ)
	{
		contact.normal = Vector(xPenetration.first < xPenetration.second ? 1.0f : -1.0f, 0.0f, 0.0f, 0.0f);
		contact.depth = minX;
	}
	// y axis is minimal penetration
	else if (minY < minX && minY < minZ)
	{
		contact.normal = Vector(0.0f, yPenetration.first < yPenetration.second ? 1.0f : -1.0f, 0.0f, 0.0f);
		contact.depth = minY;
	}
	// z axis is minimal penetration
	else
	{
		contact.normal = Vector(0.0f, 0.0f, zPenetration.first < zPenetration.second ? 1.0f : -1.0f, 0.0f);
		contact.depth = minZ;
	}

	return contact;
}

std::optional<PhysicsSystem::Contact> PhysicsSystem::CollideBoxToSphere(PhysicsComponent* pLhs, PhysicsComponent* pRhs)
{
	return std::optional<Contact>();
}

std::optional<PhysicsSystem::Contact> PhysicsSystem::CollideSphereToSphere(PhysicsComponent* pLhs, PhysicsComponent* pRhs)
{
	const Vector vLhsPosition = pLhs->GetOwner()->GetComponent<TransformComponent>()->GetAbsoluteTransform()[3];
	const Vector vRhsPosition = pRhs->GetOwner()->GetComponent<TransformComponent>()->GetAbsoluteTransform()[3];

	const float fDistanceSquared = (vRhsPosition - vLhsPosition).Length3Sq();

	const float fLhsRadiusSquared = Squared(static_cast<BoundingSphere*>(pLhs->GetCollider())->radius);
	const float fRhsRadiusSquared = Squared(static_cast<BoundingSphere*>(pRhs->GetCollider())->radius);

	if (fDistanceSquared < fLhsRadiusSquared + fRhsRadiusSquared)
	{
		Contact contact;

		contact.normal = (vRhsPosition - vLhsPosition).GetNormalized3();
		contact.depth = (fLhsRadiusSquared + fRhsRadiusSquared) - fDistanceSquared;

		return contact;
	}

	return std::nullopt;
}