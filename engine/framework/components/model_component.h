#pragma once
#include "component.h"
#include "rendering/drawable.h"

class Mesh;
class Material;
class Animator;

class ConstantBuffer;

class ModelComponent : public Component<ModelComponent>, public Drawable
{
	friend class RenderSystem;

private:
	std::string filePath;

	std::vector<std::shared_ptr<Material>> materialPtrs;
	std::vector<std::shared_ptr<Mesh>> meshPtrs;

	std::unique_ptr<Animator> pAnimator;
	std::unique_ptr<ConstantBuffer> pBoneUsageCB;

	// whole bounding box of meshes
	BoundingBox boundingBox;

	bool visible = true;
	bool castShadow = true;
	bool transparent = false;

public:
	ModelComponent();

public:
	bool LoadModel(const std::string& filePath);

	Animator* GetAnimator() const { return pAnimator.get(); }
	BoundingBox GetBoundingBox() const { return boundingBox; }

	void Draw() override;

	JSON ToJson() const override;
	void FromJson(const JSON& json) override;

	void DrawWidget() override;
};