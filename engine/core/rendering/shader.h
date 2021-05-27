#pragma once
#include "common.h"

class Shader
{
private:
	ComPtr<ID3D11VertexShader> pVertexShader;
	ComPtr<ID3D11PixelShader> pPixelShader;

	// gets initialized with vertex shader
	ComPtr<ID3D11InputLayout> pInputLayout;

public:
	bool InitializeVertexShader(const std::string& shaderPath);
	bool InitializePixelShader(const std::string& shaderPath);

	void Bind() const;

private:
	// recovers dxgi-format
	static DXGI_FORMAT GetComponentFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);
};