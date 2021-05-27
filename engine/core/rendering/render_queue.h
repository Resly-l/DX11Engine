#pragma once
#include "render_pass.h"

class Drawable;

class RenderQueue
{
private:
	std::array<std::unique_ptr<RenderPass>, (size_t)RenderPass::Tag::NUM_TAGS> passPtrs;

public:
	RenderQueue();

public:
	void AddPass(std::unique_ptr<RenderPass> pRenderPass);

	void QueueDrawable(RenderPass::Tag tag, Drawable* pDrawable);
	void Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources);
};