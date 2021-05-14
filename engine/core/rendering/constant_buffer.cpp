#include "constant_buffer.h"

void Internal::ConstantBufferBase::Bind() const
{
	if (pConstantBuffer)
	{
		if (bindFlag == ShaderBindFlag::bfBIND_VS)
		{
			Renderer::GetContext()->VSSetConstantBuffers(uSlot, 1, pConstantBuffer.GetAddressOf());
		}
		else if (bindFlag == ShaderBindFlag::bfBIND_PS)
		{
			Renderer::GetContext()->PSSetConstantBuffers(uSlot, 1, pConstantBuffer.GetAddressOf());
		}
		else if (bindFlag == ShaderBindFlag::btBIND_ALL)
		{
			Renderer::GetContext()->VSSetConstantBuffers(uSlot, 1, pConstantBuffer.GetAddressOf());
			Renderer::GetContext()->PSSetConstantBuffers(uSlot, 1, pConstantBuffer.GetAddressOf());
		}
	}
	else
	{
		Console::AddLog({ Log::Type::ltWARNING, "tried to bind null constant buffer" });
	}
}