#include "shadow_pass.h"
#include "rendering/texture.h"
#include "rendering/render_passes/lighting_pass.h"
#include "components/light_component.h"
#include "components/camera_component.h"
#include "rendering/drawable.h"
#include "rendering/render_target.h"

ShadowPass::ShadowPass()
	:
	RenderPass(RenderPass::Tag::ptSHADOW),
	shadowMaps(LightingPass::uMaxLights),
	lightTransformCB(ConstantType::ctLIGHT_TRANSFORM, ShaderBindFlag::bfBIND_VS, CameraComponent::ViewProjection{})
{
	InitializeShaders();
	InitializeRenderStates();

	faceDirections[0] = { { 1.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } };
	faceDirections[1] = { { -1.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } };
	faceDirections[2] = { { 0.0f, 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f, 0.0f } };
	faceDirections[3] = { { 0.0f, -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f } };
	faceDirections[4] = { { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } };
	faceDirections[5] = { { 0.0f, 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f } };

	projection = Matrix::Perspective(1.0f, 1.0f, 0.5f, 100.0f);
}

void ShadowPass::Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources)
{
	auto pLightResource = static_cast<LightResource*>(passResources["light_resource"].get());
	ID3D11ShaderResourceView* shadowMapPtrs[LightingPass::uMaxLights] = {};

	ID3D11ShaderResourceView* pNullSRV = nullptr;
	Renderer::GetContext()->PSSetShaderResources((uint32_t)Texture::Type::ttSHADOW, 1, &pNullSRV);

	for (int i = 0; i < pLightResource->lightProperties.size(); i++)
	{
		if (pLightResource->lightProperties[i].castShadow)
		{
			for (uint32_t uFace = 0; uFace < 6; uFace++)
			{
				shadowMaps[i].GetDepthBuffer(uFace).Clear();
				shadowMaps[i].GetDepthBuffer(uFace).BindAsTarget();

				CameraComponent::ViewProjection viewProjection;
				viewProjection.view = Matrix::LookTo(pLightResource->lightProperties[i].position, faceDirections[uFace].first, faceDirections[uFace].second);
				viewProjection.projection = projection;

				lightTransformCB.Update(viewProjection);
				lightTransformCB.Bind();

				for (auto& pDrawable : drawablePtrs)
				{
					pDrawable->Draw();
				}
			}

			shadowMapPtrs[i] = shadowMaps[i].GetShaderResourceView();
		}
	}

	Renderer::GetContext()->OMSetRenderTargets(0, nullptr, nullptr);
	Renderer::GetContext()->PSSetShaderResources(uint32_t(Texture::Type::ttSHADOW), LightingPass::uMaxLights, shadowMapPtrs);
}

void ShadowPass::InitializeShaders()
{
	shader.InitializeVertexShader("../asset/shaders/ShadowVS.cso");
	shader.initializePixelShader("");
}

void ShadowPass::InitializeRenderStates()
{
	renderState.InitializePrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderState.InitializeViewport(1024.0f, 1024.0f);

	renderState.InitializeDepthStencilState(CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} });
	renderState.InitializeBlendState(CD3D11_BLEND_DESC{ CD3D11_DEFAULT{} });

	D3D11_RASTERIZER_DESC rasterizerDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterizerDesc.DepthBias = 50;
	rasterizerDesc.SlopeScaledDepthBias = 2.6f;
	rasterizerDesc.DepthBiasClamp = 0.1f;
	renderState.InitializeRasterizerState(rasterizerDesc);
}