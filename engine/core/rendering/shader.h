#pragma once
#include "common.h"

class Shader
{
public:
	bool InitializeVertexShader(const std::string& shaderPath);
	bool initializePixelShader(const std::string& shaderPath);

	void Bind() const;

private:
	// recovers dxgi-format
	static DXGI_FORMAT GetComponentFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);

private:
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11PixelShader> pPixelShader;

	// gets initialized with vertex shader
	ComPtr<ID3D11InputLayout> pInputLayout;
};