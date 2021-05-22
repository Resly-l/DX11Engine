#pragma once
#include <DirectXMath.h>

class Matrix;

class Vector
{
public:
	Vector();
	Vector(float x, float y, float z, float w);
	Vector(const DirectX::XMVECTOR& vector);
	Vector(const DirectX::XMFLOAT2& f2);
	Vector(const DirectX::XMFLOAT3& f3);
	Vector(const DirectX::XMFLOAT4& f4);

public:
	float Length2() const;
	float Length2Sq() const;
	float Length3() const;
	float Length3Sq() const;
	float Length4() const;
	float Length4Sq() const;

	void Normalize2();
	Vector GetNormalized2() const;
	void Normalize3();
	Vector GetNormalized3() const;
	void Normalize4();
	Vector GetNormalized4() const;

	float Dot2(const Vector& other) const;
	float Dot3(const Vector& other) const;
	float Dot4(const Vector& other) const;

	float Cross2(const Vector& other) const;
	Vector Cross3(const Vector& other) const;

	Vector Lerp(const Vector& other, float fAlpha) const;
	Vector Slerp(const Vector& other, float fAlpha) const;

public:
	Vector operator-() const;

	Vector operator+(const Vector& other) const;
	Vector& operator+=(const Vector& other);
	Vector operator-(const Vector& other) const;
	Vector& operator-=(const Vector& other);

	Vector operator*(float fScalar) const;
	Vector& operator*=(float fScalar);
	Vector operator/(float fScalar) const;
	Vector& operator/=(float fScalar);

	Vector operator*(const Matrix& m) const;
	Vector& operator*=(const Matrix& m);

	float& operator[](uint32_t uIndex);
	const float& operator[](uint32_t uIndex) const;

public:
	union
	{
		struct { float x, y, z, w; };
		DirectX::XMVECTOR v;
	};
};

class Matrix
{
public:
	Matrix();
	Matrix(const DirectX::XMMATRIX& matrix);

public:
	void Transpose();
	Matrix GetTransposed() const;

	void Inverse();
	Matrix GetInversed() const;

	void Normalize();
	Matrix GetNormalized() const;

	static Matrix Identity();

	static Matrix Scaling(const Vector& scale);
	static Matrix Rotation(const Vector& angle);
	static Matrix RotationAxis(const Vector& vAxis, float fAngle);
	static Matrix RotationQuaternion(const Vector& q);
	static Matrix Translation(const Vector& position);

	static Matrix LookTo(const Vector& vPos, const Vector& vDir, const Vector& vUp);
	static Matrix Perspective(float width, float height, float fNear, float fFar);
	static Matrix PerspectiveFov(float fHFov, float fAR, float fNear, float fFar);

public:
	Matrix operator*(const Matrix& other) const;
	Matrix& operator*=(const Matrix& other);

	Vector& operator[](size_t uIndex);
	const Vector& operator[](size_t uIndex) const;

public:
	union
	{
		struct
		{
			Vector r[4];
		};
		DirectX::XMMATRIX m;
	};
};

struct BoundingVolume
{
	virtual ~BoundingVolume() = default;
};

// AABB
struct BoundingBox : public BoundingVolume
{
	Vector minExtent;
	Vector maxExtent;

	// merge rhs bounding box
	BoundingBox operator +(const BoundingBox& other) const;
	BoundingBox& operator +=(const BoundingBox& other);
};

struct BoundingSphere : public BoundingVolume
{
	Vector center;
	float radius;
};