#include "mesh.h"
#include "material.h"
#include "renderer.h"
#include "console.h"

Mesh::Mesh(const aiMesh& mesh)
	:
	uNumIndices(mesh.mNumFaces * 3)
{
	const auto vertices = ExtractVertices(mesh);

	boundingBox = CalculateBoundingBox(vertices);

	CreateVertexBuffer(vertices);
	CreateIndexBuffer(ExtractIndices(mesh));
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	:
	uNumIndices((uint32_t)indices.size())
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

	Renderer::GetContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &uVertexStride, &uVertexOffset);
	Renderer::GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	Renderer::GetContext()->DrawIndexed(uNumIndices, 0, 0);
}

std::vector<Vertex> Mesh::ExtractVertices(const aiMesh& mesh)
{
	std::vector<Vertex> vertices(mesh.mNumVertices);

	for (uint32_t uVertex = 0; uVertex < mesh.mNumVertices; uVertex++)
	{
		vertices[uVertex].position = *reinterpret_cast<float3*>(&mesh.mVertices[uVertex]);
		vertices[uVertex].normal = *reinterpret_cast<float3*>(&mesh.mNormals[uVertex]);
		vertices[uVertex].tangent = *reinterpret_cast<float3*>(&mesh.mTangents[uVertex]);
		vertices[uVertex].bitangent = *reinterpret_cast<float3*>(&mesh.mBitangents[uVertex]);
		vertices[uVertex].texcoord = *reinterpret_cast<float2*>(&mesh.mTextureCoords[0][uVertex]);
		//vertices[uVertex].boneID;
		//vertices[uVertex].boneWeight;
	}

	return vertices;
}

std::vector<uint32_t> Mesh::ExtractIndices(const aiMesh& mesh)
{
	std::vector<uint32_t> indices;
	indices.reserve(mesh.mNumFaces * 3);

	for (uint32_t uFace = 0; uFace < mesh.mNumFaces; uFace++)
	{
		indices.push_back(mesh.mFaces[uFace].mIndices[0]);
		indices.push_back(mesh.mFaces[uFace].mIndices[1]);
		indices.push_back(mesh.mFaces[uFace].mIndices[2]);
	}

	return indices;
}

BoundingBox Mesh::CalculateBoundingBox(const std::vector<Vertex>& vertices)
{
	BoundingBox boundingBox;
	boundingBox.vMinExtent = Vector(fMax, fMax, fMax, 0.0f);
	boundingBox.vMaxExtent = Vector(fMin, fMin, fMin, 0.0f);

	for (const auto& v : vertices)
	{
		if (v.position.x < boundingBox.vMinExtent.x) boundingBox.vMinExtent.x = v.position.x;
		if (v.position.y < boundingBox.vMinExtent.y) boundingBox.vMinExtent.y = v.position.y;
		if (v.position.z < boundingBox.vMinExtent.z) boundingBox.vMinExtent.z = v.position.z;

		if (v.position.x > boundingBox.vMaxExtent.x) boundingBox.vMaxExtent.x = v.position.x;
		if (v.position.y > boundingBox.vMaxExtent.y) boundingBox.vMaxExtent.y = v.position.y;
		if (v.position.z > boundingBox.vMaxExtent.z) boundingBox.vMaxExtent.z = v.position.z;
	}

	return boundingBox;
}

void Mesh::CreateVertexBuffer(const std::vector<Vertex>& vertices)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = (uint32_t)vertices.size() * uVertexStride;
	bufferDesc.StructureByteStride = uVertexStride;
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