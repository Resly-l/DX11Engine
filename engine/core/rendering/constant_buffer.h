#pragma once
#include "renderer.h"
#include "console.h"

enum class ConstantType
{
	ctCAMERA_TRANSFORM,
	ctINSTANCE_TRANSFORM,
	ctMATERIAL,
	ctLIGHT,
	ctLIGHT_COUNT,
	ctLIGHT_TRANSFORM
};

enum class ShaderBindFlag
{
	bfBIND_VS,
	bfBIND_PS,
	btBIND_ALL
};

namespace Internal
{
	class ConstantBufferBase
	{
	protected:
		uint32_t slot;
		ShaderBindFlag bindFlag;

		ComPtr<ID3D11Buffer> pConstantBuffer;

	public:
		void Bind() const;
	};
}

class ConstantBuffer : public Internal::ConstantBufferBase
{
public:
	template<typename T>
	ConstantBuffer(ConstantType type, ShaderBindFlag bindFlag, const T& constant);

public:
	template<typename T>
	void Update(const T& constant);
};

class ArrayConstantBuffer : public Internal::ConstantBufferBase
{
private:
	size_t maxArraySize;

public:
	template<typename T>
	ArrayConstantBuffer(ConstantType type, ShaderBindFlag bindFlag, const T& constant, size_t maxArraySize);

public:
	template<typename T>
	void Update(const std::vector<T>& constantArray);
};



template<typename T>
ConstantBuffer::ConstantBuffer(ConstantType type, ShaderBindFlag bindFlag, const T& constant)
{
	this->slot = uint32_t(type);
	this->bindFlag = bindFlag;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(T);
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &constant;

	if (FAILED(Renderer::GetDevice()->CreateBuffer(&bufferDesc, &sd, &pConstantBuffer)))
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to create constant buffer" });
	}
}

template<typename T>
void ConstantBuffer::Update(const T& constant)
{
	D3D11_MAPPED_SUBRESOURCE mr = {};
	if (SUCCEEDED(Renderer::GetContext()->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mr)))
	{
		memcpy(mr.pData, &constant, sizeof(T));
		Renderer::GetContext()->Unmap(pConstantBuffer.Get(), 0);
	}
	else
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to map constant buffer" });
	}
}

template<typename T>
ArrayConstantBuffer::ArrayConstantBuffer(ConstantType type, ShaderBindFlag bindFlag, const T& constant, size_t maxArraySize)
	:
	maxArraySize(maxArraySize)
{
	this->slot = uint32_t(type);
	this->bindFlag = bindFlag;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(T) * (uint32_t)maxArraySize;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd = {};
	std::vector<T> constantArray(maxArraySize);
	sd.pSysMem = constantArray.data();

	if (FAILED(Renderer::GetDevice()->CreateBuffer(&bufferDesc, &sd, &pConstantBuffer)))
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to create array constant buffer" });
	}
}

template<typename T>
void ArrayConstantBuffer::Update(const std::vector<T>& constantArray)
{
	if (constantArray.size() > maxArraySize)
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to update array constant buffer, new constant array size is bigger than max array size" });
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mr = {};
	if (SUCCEEDED(Renderer::GetContext()->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mr)))
	{
		memcpy(mr.pData, constantArray.data(), sizeof(T) * constantArray.size());
		Renderer::GetContext()->Unmap(pConstantBuffer.Get(), 0);
	}
	else
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to map array constant buffer" });
	}
}