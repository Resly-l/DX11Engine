#pragma once
#include "component.h"

class TransformComponent :public Component<TransformComponent>
{
public:
	static Vector RotationMatrixToEuler(const Matrix& rotation);

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

	Vector GetAbsolutePosition() const;
	Vector GetAbsoluteAngle() const;
	Vector GetAbsoluteScale() const;

	Matrix GetParentAbsoluteTransform() const;
	Matrix GetRelativeTransform() const;
	Matrix GetAbsoluteTransform() const;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	Vector relativePosition;
	Vector relativeAngle;
	Vector relativeScale = { 1.0f, 1.0f, 1.0f, 0.0f };

	// hierarchy concatenation
	bool passTranslation = true;
	bool passRotation = true;
	bool passScale = true;
};