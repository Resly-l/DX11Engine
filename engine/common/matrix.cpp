#include "matrix.h"

Matrix::Matrix()
	:
	m(DirectX::XMMatrixIdentity())
{}

Matrix::Matrix(const DirectX::XMMATRIX& matrix)
	:
	m(matrix)
{}

void Matrix::Transpose()
{
	m = DirectX::XMMatrixTranspose(m);
}

Matrix Matrix::GetTransposed() const
{
	return DirectX::XMMatrixTranspose(m);
}

void Matrix::Inverse()
{
	m = DirectX::XMMatrixInverse(nullptr, m);
}

Matrix Matrix::GetInversed() const
{
	return DirectX::XMMatrixInverse(nullptr, m);
}

void Matrix::Normalize()
{
	m.r[0] = DirectX::XMVector3Normalize(m.r[0]);
	m.r[1] = DirectX::XMVector3Normalize(m.r[1]);
	m.r[2] = DirectX::XMVector3Normalize(m.r[2]);
}

Matrix Matrix::GetNormalized() const
{
	Matrix res = m;

	res.m.r[0] = DirectX::XMVector3Normalize(res.m.r[0]);
	res.m.r[1] = DirectX::XMVector3Normalize(res.m.r[1]);
	res.m.r[2] = DirectX::XMVector3Normalize(res.m.r[2]);

	return res;
}

Matrix Matrix::Identity()
{
	return DirectX::XMMatrixIdentity();
}

Matrix Matrix::Scaling(const Vector& vScale)
{
	return DirectX::XMMatrixScalingFromVector(vScale.v);
}

Matrix Matrix::Rotation(const Vector& vAngle)
{
	return DirectX::XMMatrixRotationRollPitchYawFromVector(vAngle.v);
}

Matrix Matrix::RotationAxis(const Vector& vAxis, float fAngle)
{
	return DirectX::XMMatrixRotationAxis(vAxis.v, fAngle);
}

Matrix Matrix::RotationQuaternion(const Vector& q)
{
	return DirectX::XMMatrixRotationQuaternion(q.v);
}

Matrix Matrix::Translation(const Vector& vPosition)
{
	return DirectX::XMMatrixTranslationFromVector(vPosition.v);
}

Matrix Matrix::LookTo(const Vector& vPos, const Vector& vDir, const Vector& vUp)
{
	return DirectX::XMMatrixLookToLH(vPos.v, vDir.v, vUp.v);
}

Matrix Matrix::Perspective(float fWidth, float fHeight, float fNear, float fFar)
{
	return DirectX::XMMatrixPerspectiveLH(fWidth, fHeight, fNear, fFar);
}

Matrix Matrix::PerspectiveFov(float fHFov, float fAR, float fNear, float fFar)
{
	return DirectX::XMMatrixPerspectiveFovLH(fHFov / fAR, fAR, fNear, fFar);
}

Matrix Matrix::operator*(const Matrix& other) const
{
	return DirectX::XMMatrixMultiply(m, other.m);
}

Matrix& Matrix::operator*=(const Matrix& other)
{
	return *this = DirectX::XMMatrixMultiply(m, other.m);
}

Vector& Matrix::operator[](size_t uIndex)
{
	return r[uIndex];
}

const Vector& Matrix::operator[](size_t uIndex) const
{
	return r[uIndex];
}
