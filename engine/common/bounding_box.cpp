#include "bounding_box.h"

std::array<Vector, 8> BoundingBox::GetCorners() const
{
	std::array<Vector, 8> corners;

	corners[0] = Vector(vMinExtent.x, vMaxExtent.y, vMinExtent.z, 0.0f);
	corners[1] = Vector(vMaxExtent.x, vMaxExtent.y, vMinExtent.z, 0.0f);
	corners[2] = Vector(vMaxExtent.x, vMinExtent.y, vMinExtent.z, 0.0f);
	corners[3] = vMinExtent;

	corners[4] = Vector(vMinExtent.x, vMaxExtent.y, vMaxExtent.z, 0.0f);
	corners[5] = vMaxExtent;
	corners[6] = Vector(vMaxExtent.x, vMinExtent.y, vMaxExtent.z, 0.0f);
	corners[7] = Vector(vMinExtent.x, vMinExtent.y, vMaxExtent.z, 0.0f);

	return corners;
}

std::pair<std::array<Vector, 8>, std::array<uint32_t, 24>> BoundingBox::GetIndexedLines() const
{
	std::array<uint32_t, 24> indices = {
		0,1, 1,2, 2,3, 3,0,
		4,5, 5,6, 6,7, 7,4,
		0,4, 1,5, 2,6, 3,7
	};

	return std::make_pair(GetCorners(), indices);
}

std::pair<std::array<Vector, 8>, std::array<uint32_t, 36>> BoundingBox::GetIndexedTriangles() const
{
	std::array<uint32_t, 36> indices = {
		0,1,2, 2,3,0,
		4,0,3, 3,7,4,
		5,4,7, 7,6,5,
		1,5,6, 6,2,1,
		4,5,1, 1,0,4,
		6,7,3, 3,2,6
	};

	return std::make_pair(GetCorners(), indices);
}

BoundingBox BoundingBox::operator+(const BoundingBox& other) const
{
	BoundingBox boundingBox;

	boundingBox.vMinExtent.x = vMinExtent.x < other.vMinExtent.x ? vMinExtent.x : other.vMinExtent.x;
	boundingBox.vMinExtent.y = vMinExtent.y < other.vMinExtent.y ? vMinExtent.y : other.vMinExtent.y;
	boundingBox.vMinExtent.z = vMinExtent.z < other.vMinExtent.z ? vMinExtent.z : other.vMinExtent.z;

	boundingBox.vMaxExtent.x = vMaxExtent.x > other.vMaxExtent.x ? vMaxExtent.x : other.vMaxExtent.x;
	boundingBox.vMaxExtent.y = vMaxExtent.y > other.vMaxExtent.y ? vMaxExtent.y : other.vMaxExtent.y;
	boundingBox.vMaxExtent.z = vMaxExtent.z > other.vMaxExtent.z ? vMaxExtent.z : other.vMaxExtent.z;

	return boundingBox;
}

BoundingBox& BoundingBox::operator+=(const BoundingBox& other)
{
	return *this = *this + other;
}