#pragma once
#include <DirectXMath.h>
#include "vector.h"

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

	static Matrix Scaling(const Vector& vScale);
	static Matrix Rotation(const Vector& vAngle);
	static Matrix RotationAxis(const Vector& vAxis, float fAngle);
	static Matrix RotationQuaternion(const Vector& q);
	static Matrix Translation(const Vector& vPosition);

	static Matrix LookTo(const Vector& vPos, const Vector& vDir, const Vector& vUp);
	static Matrix Perspective(float fWidth,float fHeight, float fNear, float fFar);
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