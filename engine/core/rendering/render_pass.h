#pragma once
#include "shader.h"
#include "render_state.h"

class Mesh;
class Resource;
class Drawable;

class RenderPass
{
public:
	// tag is used as an index in render queue
	enum class Tag
	{
		ptSHADOW,
		ptGBUFFER,
		ptLIGHTING,
		ptSKY,
		ptALPHA,
		ptDEBUG_LINE,
		ptUI,

		NUM_TAGS
	};

protected:
	const Tag tag;

	Shader shader;
	RenderState renderState;

	std::vector<Drawable*> drawablePtrs;

public:
	RenderPass(Tag tag) : tag(tag) {}
	virtual ~RenderPass() = default;

public:
	Tag GetTag() const { return tag; }

	void AddDrawable(Drawable* pDrawable) { drawablePtrs.push_back(pDrawable); }
	void Clear() { drawablePtrs.clear(); }

	// used by render queue when binding
	const Shader& GetShader() const { return shader; }
	const RenderState& GetRenderState() const { return renderState; }

	virtual void Render(std::unordered_map<std::string, std::shared_ptr<Resource>>& passResources) = 0;
};