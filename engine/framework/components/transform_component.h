#pragma once
#include "component.h"

class TransformComponent :public Component<TransformComponent>
{
private:
	Vector position;
	Vector angle;
	Vector scale = { 1.0f, 1.0f, 1.0f, 0.0f };

	// hierarchy concatenation
	bool passTranslation = true;
	bool passRotation = true;
	bool passScale = true;

public:
	void SetPosition(const Vector& position) { this->position = position; }
	void SetAngle(const Vector& angle) { this->angle = angle; }

	Vector GetPosition() const { return position; }
	Vector GetAngle() const { return angle; }

	Matrix GetTransformMatrix() const;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};