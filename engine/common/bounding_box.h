#pragma once
#include <array>
#include <utility>
#include "vector.h"
#include "matrix.h"

class BoundingBox
{
public:
	Vector vMinExtent;
	Vector vMaxExtent;
	Matrix mRotation;

public:
	std::array<Vector, 8> GetCorners() const;
	std::pair<std::array<Vector, 8>, std::array<uint32_t, 24>> GetIndexedLines() const;
	std::pair<std::array<Vector, 8>, std::array<uint32_t, 36>> GetIndexedTriangles() const;

	// merge rhs bounding box
	// two boxes should be aabb otherwise rotation will be lost
	BoundingBox operator +(const BoundingBox& other) const;
	BoundingBox& operator +=(const BoundingBox& other);
};