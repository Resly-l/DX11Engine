#include "maths.h"

Vector::Vector()
	:
	v(DirectX::XMVectorZero())
{}

Vector::Vector(float x, float y, float z, float w)
	:
	x(x),
	y(y),
	z(z),
	w(w)
{}

Vector::Vector(const DirectX::XMVECTOR& vector)
	:
	v(vector)
{}

Vector::Vector(const DirectX::XMFLOAT2& f2)
	:
	x(f2.x),
	y(f2.y),
	z(0.0f),
	w(0.0f)
{}

Vector::Vector(const DirectX::XMFLOAT3& f3)
	:
	x(f3.x),
	y(f3.y),
	z(f3.y),
	w(0.0f)
{}

Vector::Vector(const DirectX::XMFLOAT4& f4)
	:
	x(f4.x),
	y(f4.y),
	z(f4.z),
	w(f4.w)
{}

float Vector::Length2() const
{
	return DirectX::XMVector2Length(v).m128_f32[0];
}

float Vector::Length2Sq() const
{
	return DirectX::XMVector2LengthSq(v).m128_f32[0];
}

float Vector::Length3() const
{
	return DirectX::XMVector3Length(v).m128_f32[0];
}

float Vector::Length3Sq() const
{
	return DirectX::XMVector3LengthSq(v).m128_f32[0];
}

float Vector::Length4() const
{
	return DirectX::XMVector4Length(v).m128_f32[0];
}

float Vector::Length4Sq() const
{
	return DirectX::XMVector4LengthSq(v).m128_f32[0];
}

void Vector::Normalize2()
{
	v = DirectX::XMVector2Normalize(v);
}

Vector Vector::GetNormalized2() const
{
	return DirectX::XMVector2Normalize(v);
}

void Vector::Normalize3()
{
	v = DirectX::XMVector3Normalize(v);
}

Vector Vector::GetNormalized3() const
{
	return DirectX::XMVector3Normalize(v);
}

void Vector::Normalize4()
{
	v = DirectX::XMVector4Normalize(v);
}

Vector Vector::GetNormalized4() const
{
	return DirectX::XMVector4Normalize(v);
}

float Vector::Dot2(const Vector& other) const
{
	return DirectX::XMVector2Dot(v, other.v).m128_f32[0];
}

float Vector::Dot3(const Vector& other) const
{
	return DirectX::XMVector3Dot(v, other.v).m128_f32[0];
}

float Vector::Dot4(const Vector& other) const
{
	return DirectX::XMVector4Dot(v, other.v).m128_f32[0];
}

float Vector::Cross2(const Vector& other) const
{
	return DirectX::XMVector2Cross(v, other.v).m128_f32[0];
}

Vector Vector::Cross3(const Vector& other) const
{
	return DirectX::XMVector3Cross(v, other.v);
}

Vector Vector::Lerp(const Vector& other, float fAlpha) const
{
	return *this + (other - *this) * fAlpha;
}

Vector Vector::Slerp(const Vector& other, float fAlpha) const
{
	return DirectX::XMQuaternionSlerp(v, other.v, fAlpha);
}

Vector Vector::operator-() const
{
	return DirectX::operator-(v);
}

Vector Vector::operator+(const Vector& other) const
{
	return DirectX::operator+(v, other.v);
}

Vector& Vector::operator+=(const Vector& other)
{
	return *this = DirectX::operator+(v, other.v);
}

Vector Vector::operator-(const Vector& other) const
{
	return DirectX::operator-(v, other.v);
}

Vector& Vector::operator-=(const Vector& other)
{
	return *this = DirectX::operator-(v, other.v);
}

Vector Vector::operator*(float fScalar) const
{
	return DirectX::operator*(v, fScalar);
}

Vector& Vector::operator*=(float fScalar)
{
	return *this = DirectX::operator*(v, fScalar);
}

Vector Vector::operator/(float fScalar) const
{
	return DirectX::operator/(v, fScalar);
}

Vector& Vector::operator/=(float fScalar)
{
	return *this = DirectX::operator/(v, fScalar);
}

Vector Vector::operator*(const Matrix& m) const
{
	return DirectX::XMVector3Transform(v, m.m);
}

Vector& Vector::operator*=(const Matrix& m)
{
	return *this = DirectX::XMVector3Transform(v, m.m);
}

float& Vector::operator[](uint32_t uIndex)
{
	return v.m128_f32[uIndex];
}

const float& Vector::operator[](uint32_t uIndex) const
{
	return v.m128_f32[uIndex];
}

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

Matrix Matrix::Scaling(const Vector& scale)
{
	return DirectX::XMMatrixScalingFromVector(scale.v);
}

Matrix Matrix::Rotation(const Vector& angle)
{
	return DirectX::XMMatrixRotationRollPitchYawFromVector(angle.v);
}

Matrix Matrix::RotationAxis(const Vector& vAxis, float fAngle)
{
	return DirectX::XMMatrixRotationAxis(vAxis.v, fAngle);
}

Matrix Matrix::RotationQuaternion(const Vector& q)
{
	return DirectX::XMMatrixRotationQuaternion(q.v);
}

Matrix Matrix::Translation(const Vector& position)
{
	return DirectX::XMMatrixTranslationFromVector(position.v);
}

Matrix Matrix::LookTo(const Vector& vPos, const Vector& vDir, const Vector& vUp)
{
	return DirectX::XMMatrixLookToLH(vPos.v, vDir.v, vUp.v);
}

Matrix Matrix::Perspective(float width, float height, float fNear, float fFar)
{
	return DirectX::XMMatrixPerspectiveLH(width, height, fNear, fFar);
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

BoundingBox BoundingBox::operator+(const BoundingBox& other) const
{
	BoundingBox boundingBox;

	boundingBox.minExtent.x = minExtent.x < other.minExtent.x ? minExtent.x : other.minExtent.x;
	boundingBox.minExtent.y = minExtent.y < other.minExtent.y ? minExtent.y : other.minExtent.y;
	boundingBox.minExtent.z = minExtent.z < other.minExtent.z ? minExtent.z : other.minExtent.z;

	boundingBox.maxExtent.x = maxExtent.x > other.maxExtent.x ? maxExtent.x : other.maxExtent.x;
	boundingBox.maxExtent.y = maxExtent.y > other.maxExtent.y ? maxExtent.y : other.maxExtent.y;
	boundingBox.maxExtent.z = maxExtent.z > other.maxExtent.z ? maxExtent.z : other.maxExtent.z;

	return boundingBox;
}

BoundingBox& BoundingBox::operator+=(const BoundingBox& other)
{
	return *this = *this + other;
}