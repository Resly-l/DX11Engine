#pragma once
#include "component.h"

class ConstantBuffer;

class CameraComponent : public Component<CameraComponent>
{
public:
	struct ViewProjection
	{
		Matrix view;
		Matrix perspective;
	};

private:
	static std::unique_ptr<ConstantBuffer> pViewProjectionCB;

	float horizontalFOV = (float)pi * 0.5f;
	float nearPlaneDistance = 0.001f;
	float farPlaneDistance = 100.0f;

	float aspectRatio = 1.777777f;
	
public:
	CameraComponent();

public:
	Matrix GetViewMatrix() const;
	Matrix GetProjectionMatrix() const;

	void Bind();

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};