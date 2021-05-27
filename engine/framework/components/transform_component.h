#pragma once
#include "component.h"

class ConstantBuffer;

class TransformComponent :public Component<TransformComponent>
{
private:
	static std::unique_ptr<ConstantBuffer> pConstantBuffer;

	Vector relativePosition;
	Vector relativeAngle;
	Vector relativeScale = { 1.0f, 1.0f, 1.0f, 0.0f };

	// hierarchy concatenation
	bool passTranslation = true;
	bool passRotation = true;
	bool passScale = true;

public:
	TransformComponent();

public:
	void SetRelativePosition(const Vector& position) { relativePosition = position; }
	void SetRelativeAngle(const Vector& angle) { relativeAngle = angle; }
	void SetRelativeScale(const Vector& scale) { relativeScale = scale; }

	void SetAbsolutePosition(const Vector& position);
	void SetAbsoluteAngle(const Vector& angle);
	void SetAbsoluteScale(const Vector& scale);

	void LookAt(const Vector& position);

	Vector GetRelativePosition() const { return relativePosition; }
	Vector GetRelativeAngle() const { return relativeAngle; }
	Vector GetRelativeScale() const { return relativeScale; }

	Matrix GetParentAbsoluteTransform() const;
	Matrix GetRelativeTransform() const;
	Matrix GetAbsoluteTransform() const;

	void Bind();

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};