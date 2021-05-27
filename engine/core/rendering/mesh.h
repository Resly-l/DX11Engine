#pragma once
#include "resource.h"

struct Vertex
{
	float3 position;
	float3 normal;
	float3 tangent;
	float3 bitangent;
	float2 texcoord;
	uint4 boneID;
	float4 boneWeight;
};

class Material;
class Animator;

class Mesh : public Resource
{
private:
	ComPtr<ID3D11Buffer> pVertexBuffer;
	static constexpr uint32_t vertexStride = sizeof(Vertex);
	static constexpr uint32_t vertexOffset = 0;

	ComPtr<ID3D11Buffer> pIndexBuffer;
	uint32_t numIndices;

	BoundingBox boundingBox;

	std::shared_ptr<Material> pMaterial;

public:
	Mesh(const aiMesh& ai_mesh, Animator* pAnimator);
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);

public:
	const BoundingBox& GetBoundingBox() const { return boundingBox; }

	void SetMaterial(std::shared_ptr<Material> pMaterial);
	Material* GetMaterial() const;

	void Draw() const;

private:
	static std::vector<Vertex> ExtractVertices(const aiMesh& ai_mesh, Animator* pAnimator);
	static std::vector<uint32_t> ExtractIndices(const aiMesh& ai_mesh);

	static BoundingBox CalculateBoundingBox(const std::vector<Vertex>& vertices);

	void CreateVertexBuffer(const std::vector<Vertex>& vertices);
	void CreateIndexBuffer(const std::vector<uint32_t>& indices);
};