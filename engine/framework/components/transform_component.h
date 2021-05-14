#pragma once
#include "component.h"

class TransformComponent :public Component<TransformComponent>
{
private:
	struct PassDown
	{
		bool bTranslation = true;
		bool bRotation = true;
		bool bScale = true;
	};

private:
	Vector vPosition;
	Vector vAngle;
	Vector vScale = { 1.0f, 1.0f, 1.0f, 0.0f };

	PassDown passdown;

public:
	void SetPosition(Vector vPosition) { this->vPosition = vPosition; }
	void SetAngle(Vector vAngle) { this->vAngle = vAngle; }

	Vector GetPosition() const { return vPosition; }
	Vector GetAngle() const { return vAngle; }

	Matrix GetTransformMatrix() const;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};