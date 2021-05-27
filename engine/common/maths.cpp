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

Vector Vector::operator*(const Vector& v) const
{
	return Vector(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector& Vector::operator*=(const Vector& v)
{
	return *this = *this * v;
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

Decomposed Matrix::Decompose() const
{
	Decomposed result;

	result.scale.x = Vector(r[0][0], r[0][1], r[0][2], 0.0f).Length3();
	result.scale.y = Vector(r[1][0], r[1][1], r[1][2], 0.0f).Length3();
	result.scale.z = Vector(r[2][0], r[2][1], r[2][2], 0.0f).Length3();

	Matrix rotation = m;

	rotation[0][0] /= result.scale.x;
	rotation[1][0] /= result.scale.x;
	rotation[2][0] /= result.scale.x;

	rotation[0][1] /= result.scale.y;
	rotation[1][1] /= result.scale.y;
	rotation[2][1] /= result.scale.y;

	rotation[0][2] /= result.scale.z;
	rotation[1][2] /= result.scale.z;
	rotation[2][2] /= result.scale.z;

	if (rotation[2][1] < 1.0f)
	{
		if (rotation[2][1] > -1.0f)
		{
			result.angle.x = -asinf(rotation[2][1]);
			result.angle.z = -atan2f(-rotation[0][1], rotation[1][1]);
			result.angle.y = -atan2f(-rotation[2][0], rotation[2][2]);
		}
		else
		{
			result.angle.x = 3.1415926535f * 0.5f;
			result.angle.z = atan2f(rotation[0][2], rotation[0][0]);
			result.angle.y = 0;
		}
	}
	else
	{
		result.angle.x = -3.1415926535f * 0.5f;
		result.angle.z = -atan2f(rotation[0][2], rotation[0][0]);
		result.angle.y = 0.0f;
	}

	result.position = r[3];

	return result;
}

Matrix Matrix::Identity()
{
	return DirectX::XMMatrixIdentity();
}

Matrix Matrix::Interpolate(const Matrix& other, float alpha)
{
	const auto lhsDecomposed = this->Decompose();
	const auto rhsDecomposed = other.Decompose();

	const Vector lhsQuaternion = DirectX::XMQuaternionRotationRollPitchYawFromVector(lhsDecomposed.angle.v);
	const Vector rhsQuaternion = DirectX::XMQuaternionRotationRollPitchYawFromVector(rhsDecomposed.angle.v);

	return Matrix::Scaling(lhsDecomposed.scale.Lerp(rhsDecomposed.scale, alpha))
		* Matrix::RotationQuaternion(lhsQuaternion.Slerp(rhsQuaternion, alpha))
		* Matrix::Translation(lhsDecomposed.position.Lerp(rhsDecomposed.position, alpha));
}

Matrix Matrix::Scaling(const Vector& scale)
{
	return DirectX::XMMatrixScalingFromVector(scale.v);
}

Matrix Matrix::Rotation(const Vector& angle)
{
	return DirectX::XMMatrixRotationRollPitchYawFromVector(angle.v);
}

Matrix Matrix::RotationAxis(const Vector& axis, float angle)
{
	return DirectX::XMMatrixRotationAxis(axis.v, angle);
}

Matrix Matrix::RotationQuaternion(const Vector& quaternion)
{
	return DirectX::XMMatrixRotationQuaternion(quaternion.v);
}

Matrix Matrix::Translation(const Vector& position)
{
	return DirectX::XMMatrixTranslationFromVector(position.v);
}

Matrix Matrix::LookTo(const Vector& position, const Vector& direction, const Vector& upside)
{
	return DirectX::XMMatrixLookToLH(position.v, direction.v, upside.v);
}

Matrix Matrix::Perspective(float width, float height, float near, float far)
{
	return DirectX::XMMatrixPerspectiveLH(width, height, near, far);
}

Matrix Matrix::PerspectiveFov(float horizontalFov, float aspectRatio, float near, float far)
{
	return DirectX::XMMatrixPerspectiveFovLH(horizontalFov / aspectRatio, aspectRatio, near, far);
}

Matrix Matrix::Orthogonal(float width, float height, float near, float far)
{
	return DirectX::XMMatrixOrthographicLH(width, height, near, far);
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