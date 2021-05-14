#include "render_queue.h"
#include "render_passes/shadow_pass.h"
#include "render_passes/gbuffer_pass.h"
#include "render_passes/lighting_pass.h"
#include "render_passes/skybox_pass.h"
#include "resource.h"
#include "drawable.h"

RenderQueue::RenderQueue()
{
	// default passes
	{
		AddPass(std::make_unique<ShadowPass>());
		AddPass(std::make_unique<GBufferPass>());
		AddPass(std::make_unique<LightingPass>());
		AddPass(std::make_unique<SkyboxPass>());
	}
}

void RenderQueue::AddPass(std::unique_ptr<RenderPass> pRenderPass)
{
	passPtrs[(size_t)pRenderPass->GetTag()] = std::move(pRenderPass);
}

void RenderQueue::QueueDrawable(RenderPass::Tag tag, Drawable* pDrawable)
{
	if (passPtrs[(uint32_t)tag])
	{
		passPtrs[(uint32_t)tag]->AddDrawable(pDrawable);
	}
}

void RenderQueue::Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources)
{
	for (auto& pPass : passPtrs)
	{
		if (pPass != nullptr)
		{
			pPass->GetShader().Bind();
			pPass->GetRenderState().Bind();

			pPass->Render(passResources);

			pPass->Clear();
		}
	}
}