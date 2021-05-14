#pragma once
#include "component.h"
#include "rendering/drawable.h"

class Mesh;
class Material;
class RenderQueue;
class ConstantBuffer;

class RenderComponent : public Component<RenderComponent>, public Drawable
{
private:
	static std::unique_ptr<ConstantBuffer> pTransformCB;

	std::string filePath;

	std::vector<std::shared_ptr<Material>> materialPtrs;
	std::vector<std::shared_ptr<Mesh>> meshPtrs;

	// whole bounding box of meshes
	BoundingBox boundingBox;

	bool bCastShadow = true;
	bool bAlpha = false;

public:
	RenderComponent();

public:
	bool LoadModel(const std::string& filePath);

	void Submit(RenderQueue& renderQueue);
	void Draw() override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};