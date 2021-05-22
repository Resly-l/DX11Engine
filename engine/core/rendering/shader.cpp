#include "shader.h"
#include "renderer.h"

bool Shader::InitializeVertexShader(const std::string& shaderPath)
{
	ComPtr<ID3DBlob> pBlob;
	if (SUCCEEDED(D3DReadFileToBlob(ToWideString(shaderPath).c_str(), &pBlob)))
	{
		if (SUCCEEDED(Renderer::GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader)))
		{
			ComPtr<ID3D11ShaderReflection> pShaderReflection;
			D3D11_SHADER_DESC shaderDesc = {};
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescs;

			D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection, &pShaderReflection);
			pShaderReflection->GetDesc(&shaderDesc);

			for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
			{
				D3D11_SIGNATURE_PARAMETER_DESC spDesc = {};
				pShaderReflection->GetInputParameterDesc(i, &spDesc);

				D3D11_INPUT_ELEMENT_DESC desc = {};
				desc.SemanticName = spDesc.SemanticName;
				desc.SemanticIndex = spDesc.SemanticIndex;
				desc.InputSlot = 0;
				desc.Format = GetComponentFormat(spDesc.ComponentType, spDesc.Mask);
				desc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;

				inputElementDescs.push_back(std::move(desc));
			}

			Renderer::GetDevice()->CreateInputLayout(inputElementDescs.data(), (UINT)inputElementDescs.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
			return true;
		}
	}
	return false;
}

bool Shader::initializePixelShader(const std::string& shaderPath)
{
	ComPtr<ID3DBlob> pBlob;
	if (SUCCEEDED(D3DReadFileToBlob(ToWideString(shaderPath).c_str(), &pBlob)))
	{
		if (SUCCEEDED(Renderer::GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader)))
		{
			return true;
		}
	}
	return false;
}

void Shader::Bind() const
{
	if (pVertexShader)
	{
		Renderer::GetContext()->IASetInputLayout(pInputLayout.Get());
		Renderer::GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);
	}

	// pixel shader can be null
	Renderer::GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0);
}

DXGI_FORMAT Shader::GetComponentFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
	if (mask == 1)
	{
		if (type == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32_UINT;
		else if (type == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32_SINT;
		else if (type == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32_FLOAT;
	}
	else if (mask <= 3)
	{
		if (type == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32_UINT;
		else if (type == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32_SINT;
		else if (type == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32_FLOAT;
	}
	else if (mask <= 7)
	{
		if (type == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32_UINT;
		else if (type == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32_SINT;
		else if (type == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32_FLOAT;
	}
	else if (mask <= 15)
	{
		if (type == D3D_REGISTER_COMPONENT_UINT32) return DXGI_FORMAT_R32G32B32A32_UINT;
		else if (type == D3D_REGISTER_COMPONENT_SINT32) return DXGI_FORMAT_R32G32B32A32_SINT;
		else if (type == D3D_REGISTER_COMPONENT_FLOAT32) return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	return DXGI_FORMAT_UNKNOWN;
}