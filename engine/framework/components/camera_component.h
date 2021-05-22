#pragma once
#include "component.h"
#include "rendering/constant_buffer.h"

class CameraComponent : public Component<CameraComponent>
{
public:
	struct ViewProjection
	{
		Matrix view;
		Matrix projection;
	};

private:
	float horizontalFOV = (float)pi * 0.5f;
	float nearPlaneDistance = 0.001f;
	float farPlaneDistance = 100.0f;

	// 0.0f = swap chain aspect ratio
	float aspectRatio = 1.777777f;

	ConstantBuffer viewProjectionCB;
	
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