#include "physics_component.h"
#include "entity.h"
#include "transform_component.h"
#include "model_component.h"

std::vector<std::string> PhysicsComponent::boundingVolumeNames = {
	"BoundingBox",
	"BoundingSphere"
};

void PhysicsComponent::Integrate(double deltaSeconds)
{
	if (auto pTransformComponent = GetOwner()->GetComponent<TransformComponent>())
	{
		// sympletic euler integration
		velocity += (force * inverseMass) * (float)deltaSeconds;
		memset(&force, 0, sizeof(force));

		pTransformComponent->SetAbsolutePosition(pTransformComponent->GetAbsoluteTransform().Decompose().position + velocity * (float)deltaSeconds);
	}
}

JSON PhysicsComponent::ToJson() const
{
	JSON json;

	json["collider_index"] = colliderIndex;

	switch (colliderIndex)
	{
	case 0:
		json["collider_min_extent"] = static_cast<BoundingBox*>(pCollider.get())->minExtent;
		json["collider_max_extent"] = static_cast<BoundingBox*>(pCollider.get())->maxExtent;
		break;
	case 1:
		json["collider_center"] = static_cast<BoundingSphere*>(pCollider.get())->center;
		json["collider_radius"] = static_cast<BoundingSphere*>(pCollider.get())->radius;
		break;
	}

	json["mass"] = mass;
	json["friction"] = friction;
	json["restitution"] = restitution;

	return json;
}

void PhysicsComponent::FromJson(const JSON& json)
{
	colliderIndex = json["collider_index"];
	if (colliderIndex != -1)
	{
		switch (colliderIndex)
		{
		case 0:
		{
			auto pBox = std::make_unique<BoundingBox>();

			pBox->minExtent = json["collider_min_extent"];
			pBox->maxExtent = json["collider_max_extent"];

			pCollider = std::move(pBox);
			break;
		}
		case 1:
		{
			auto pSphere = std::make_unique<BoundingSphere>();

			pSphere->center = json["collider_center"];
			pSphere->radius = json["collider_radius"];

			pCollider = std::move(pSphere);
			break;
		}
		}
	}

	mass = json["mass"];
	inverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
	friction = json["friction"];
	restitution = json["restitution"];
}

void PhysicsComponent::DrawWidget()
{
	DrawColliderCombo();
	DrawColliderButton();
	DrawColliderWidget();

	ImGui::NewLine();
	if (ImGui::DragFloat("mass", &mass, 0.001f, 0.0f))
	{
		inverseMass = mass == 0.0f ? 0.0f : 1.0f / mass;
	}

	ImGui::DragFloat("friction", &friction, 0.001f, 0.0f);
	ImGui::DragFloat("restitution", &restitution, 0.001f, 0.0f);
	ImGui::DragFloat("gravity factor", &gravityFactor, 0.001f, 0.0f);
}

void PhysicsComponent::DrawColliderCombo()
{
	if (ImGui::BeginCombo("collider type", boundingVolumeNames[boundingVolumeIndex].c_str()))
	{
		for (size_t uIndex = 0; uIndex < boundingVolumeNames.size(); uIndex++)
		{
			if (ImGui::Selectable(boundingVolumeNames[uIndex].c_str()))
			{
				boundingVolumeIndex = uIndex;
			}
		}

		ImGui::EndCombo();
	}
}

void PhysicsComponent::DrawColliderButton()
{
	if (ImGui::Button("set collider"))
	{
		colliderIndex = boundingVolumeIndex;

		switch (colliderIndex)
		{
		case 0:
			pCollider = std::make_unique<BoundingBox>();
			break;
		case 1:
			pCollider = std::make_unique<BoundingSphere>();
			break;
		}
	}
}

void PhysicsComponent::DrawColliderWidget()
{
	if (pCollider == nullptr)
	{
		return;
	}

	ImGui::NewLine();
	switch (colliderIndex)
	{
	case 0:
	{
		auto pBoundingBox = static_cast<BoundingBox*>(pCollider.get());

		ImGui::DragFloat3("min extent", &pBoundingBox->minExtent.x, 0.01f);
		ImGui::DragFloat3("max extent", &pBoundingBox->maxExtent.x, 0.01f);

		if (auto pModelComponent = GetOwner()->GetComponent<ModelComponent>())
		{
			ImGui::NewLine();
			if (ImGui::Button("set from model"))
			{
				*pBoundingBox = pModelComponent->GetBoundingBox();
			}
		}

		break;
	}
	case 1:
	{
		auto pBoundingSphere = static_cast<BoundingSphere*>(pCollider.get());

		ImGui::DragFloat3("center", &pBoundingSphere->center.x);
		ImGui::DragFloat("radius", &pBoundingSphere->radius, 0.01f, 0.0f);

		break;
	}
	}
}