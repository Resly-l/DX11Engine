#include "mesh.h"
#include "renderer.h"
#include "material.h"
#include "animator.h"
#include "console.h"

Mesh::Mesh(const aiMesh& ai_mesh, Animator* pAnimator)
	:
	numIndices(ai_mesh.mNumFaces * 3)
{
	const auto vertices = ExtractVertices(ai_mesh, pAnimator);

	boundingBox = CalculateBoundingBox(vertices);

	CreateVertexBuffer(vertices);
	CreateIndexBuffer(ExtractIndices(ai_mesh));
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	:
	numIndices((uint32_t)indices.size())
{
	boundingBox = CalculateBoundingBox(vertices);

	CreateVertexBuffer(vertices);
	CreateIndexBuffer(indices);
}

void Mesh::SetMaterial(std::shared_ptr<Material> pMaterial)
{
	this->pMaterial = pMaterial;
}

Material* Mesh::GetMaterial() const
{
	return pMaterial.get();
}

void Mesh::Draw() const
{
	if (pMaterial) pMaterial->Bind();

	Renderer::GetContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);
	Renderer::GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	Renderer::GetContext()->DrawIndexed(numIndices, 0, 0);
}

std::vector<Vertex> Mesh::ExtractVertices(const aiMesh& ai_mesh, Animator* pAnimator)
{
	std::vector<Vertex> vertices(ai_mesh.mNumVertices);

	for (uint32_t uVertex = 0; uVertex < ai_mesh.mNumVertices; uVertex++)
	{
		vertices[uVertex].position = *reinterpret_cast<float3*>(&ai_mesh.mVertices[uVertex]);
		vertices[uVertex].normal = *reinterpret_cast<float3*>(&ai_mesh.mNormals[uVertex]);
		vertices[uVertex].tangent = *reinterpret_cast<float3*>(&ai_mesh.mTangents[uVertex]);
		vertices[uVertex].bitangent = *reinterpret_cast<float3*>(&ai_mesh.mBitangents[uVertex]);
		vertices[uVertex].texcoord = *reinterpret_cast<float2*>(&ai_mesh.mTextureCoords[0][uVertex]);
	}

	if (pAnimator == nullptr)
	{
		return vertices;
	}

	for (uint32_t i = 0; i < ai_mesh.mNumBones; i++)
	{
		auto& ai_bone = *ai_mesh.mBones[i];

		for (uint32_t j = 0; j < ai_bone.mNumWeights; j++)
		{
			// same vertex is modified only 4 times which is max weight limitation
			auto& affectedVertex = vertices[ai_bone.mWeights[j].mVertexId];

			for (uint32_t k = 0; k < 4; k++)
			{
				if (reinterpret_cast<float*>(&affectedVertex.boneWeight)[k] == 0.0f)
				{
					reinterpret_cast<uint32_t*>(&affectedVertex.boneID.x)[k] = pAnimator->GetBones().at(ai_bone.mName.C_Str()).index;
					reinterpret_cast<float*>(&affectedVertex.boneWeight)[k] = ai_bone.mWeights[j].mWeight;

					break;
				}
			}
		}
	}

	return vertices;
}

std::vector<uint32_t> Mesh::ExtractIndices(const aiMesh& ai_mesh)
{
	std::vector<uint32_t> indices;
	indices.reserve(ai_mesh.mNumFaces * 3);

	for (uint32_t uFace = 0; uFace < ai_mesh.mNumFaces; uFace++)
	{
		indices.push_back(ai_mesh.mFaces[uFace].mIndices[0]);
		indices.push_back(ai_mesh.mFaces[uFace].mIndices[1]);
		indices.push_back(ai_mesh.mFaces[uFace].mIndices[2]);
	}

	return indices;
}

BoundingBox Mesh::CalculateBoundingBox(const std::vector<Vertex>& vertices)
{
	BoundingBox boundingBox;

	for (const auto& v : vertices)
	{
		if (v.position.x < boundingBox.minExtent.x) boundingBox.minExtent.x = v.position.x;
		if (v.position.y < boundingBox.minExtent.y) boundingBox.minExtent.y = v.position.y;
		if (v.position.z < boundingBox.minExtent.z) boundingBox.minExtent.z = v.position.z;

		if (v.position.x > boundingBox.maxExtent.x) boundingBox.maxExtent.x = v.position.x;
		if (v.position.y > boundingBox.maxExtent.y) boundingBox.maxExtent.y = v.position.y;
		if (v.position.z > boundingBox.maxExtent.z) boundingBox.maxExtent.z = v.position.z;
	}

	return boundingBox;
}

void Mesh::CreateVertexBuffer(const std::vector<Vertex>& vertices)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = (uint32_t)vertices.size() * vertexStride;
	bufferDesc.StructureByteStride = vertexStride;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	if (FAILED(Renderer::GetDevice()->CreateBuffer(&bufferDesc, &sd, &pVertexBuffer)))
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to create vertex buffer" });
	}
}

void Mesh::CreateIndexBuffer(const std::vector<uint32_t>& indices)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = (uint32_t)indices.size() * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = indices.data();

	if (FAILED(Renderer::GetDevice()->CreateBuffer(&bufferDesc, &sd, &pIndexBuffer)))
	{
		Console::AddLog({ Log::Type::ltERROR, "failed to create index buffer" });
	}
}