#pragma once
#include "component.h"
#include "rendering/constant_buffer.h"

class CameraComponent : public Component<CameraComponent>
{
public:
	struct ViewProjection
	{
		Matrix mView;
		Matrix mProjection;
	};

private:
	float fHorizontalFOV = fPI * 0.5f;
	float fNearPlaneDistance = 0.001f;
	float fFarPlaneDistance = 100.0f;

	// 0.0f = swap chain aspect ratio
	float fAspectRatio = 0.0f;

	ConstantBuffer viewProjectionCB;
	
public:
	CameraComponent();

public:
	void SetHorizontalFov(float fFOV) { fHorizontalFOV = fFOV; }
	void SetNearPlaneDistance(float fDistance) { fNearPlaneDistance = fDistance; }
	void SetFarPlaneDistance(float fDistance) { fFarPlaneDistance = fDistance; }

	void SetAspectRatio(float fRatio) { fAspectRatio = fRatio; }

	float GetHorizontalFov() const { return fHorizontalFOV; }
	float GetNearPlaneDistance() const { return fNearPlaneDistance; }
	float GetFarPlaneDistance() const { return fFarPlaneDistance; }

	Matrix GetViewMatrix() const;
	Matrix GetProjectionMatrix() const;

	void Bind();

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};