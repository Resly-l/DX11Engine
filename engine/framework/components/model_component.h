#pragma once
#include "component.h"
#include "rendering/drawable.h"

class Mesh;
class Material;
class ConstantBuffer;

class ModelComponent : public Component<ModelComponent>, public Drawable
{
	friend class RenderSystem;

public:
	ModelComponent();

public:
	bool LoadModel(const std::string& filePath);

	BoundingBox GetBoundingBox() const { return boundingBox; }

	void Draw() override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;

private:
	static std::unique_ptr<ConstantBuffer> pTransformCB;

	std::string filePath;

	std::vector<std::shared_ptr<Material>> materialPtrs;
	std::vector<std::shared_ptr<Mesh>> meshPtrs;

	// whole bounding box of meshes
	BoundingBox boundingBox;

	bool visible = true;
	bool castShadow = true;
	bool transparent = false;
};