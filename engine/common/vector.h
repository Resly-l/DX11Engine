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