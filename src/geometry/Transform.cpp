#include "Transform.hpp"
#include <vector>
#include "../math/Math.hpp"

Transform::Transform(const Vec3& position, const Vec3& rotation, const Vec3& scale) : _transformMatrix(Mat4()), _position(position), _rotation(rotation), _scale(scale)
{
	setPosition(position);
	setRotation(rotation);
	setScale(scale);
}

#pragma region Getters

const Mat4& Transform::getTransformMatrix() const
{
	return _transformMatrix;
}

const Vec3& Transform::getPosition() const
{
	return _position;
}

const Vec3& Transform::getRotation() const
{
	return _rotation;
}

const Vec3& Transform::getScale() const
{
	return _scale;
}

#pragma endregion

#pragma region Setters

void Transform::setScale(const Vec3& value)
{
	_scale = value;
	_transformMatrix(0, 0) = value.x;
	_transformMatrix(1, 1) = value.y;
	_transformMatrix(2, 2) = value.z;
}

void Transform::setPosition(const Vec3& newPosition)
{
	Vec3 delta = newPosition - _position;
	translate(delta);
}

void Transform::setRotation(const Vec3& newRotation)
{
	_transformMatrix = Mat4();
	_transformMatrix.scale(_scale);
	_transformMatrix.translate(_position);

	Vec3 rotationRadians = radians(newRotation);
	_transformMatrix.rotate(rotationRadians);

	_rotation = newRotation;
}

#pragma endregion

#pragma region Actions

void Transform::translate(const Vec3& offset)
{
	_position += offset;
	_transformMatrix.translate(offset);
}

void Transform::rotate(const Vec3& degrees)
{
	Vec3 rads = radians(degrees);
	_rotation += rads;
	_transformMatrix.rotate(rads);
}

void Transform::scale(const Vec3& value)
{
	_scale *= value;
	_transformMatrix.scale(value);
}

void Transform::lookat(const Vec3& target)
{
    _transformMatrix = Mat4::lookAt(_position, target, Vec3(0.0f, 1.0f, 0.0f));

	// update rot
	Vec3 newRotation;
    newRotation.y = std::asin(-_transformMatrix(2, 0));
    if (std::cos(newRotation.y) != 0)
	{
        newRotation.x = std::atan2(_transformMatrix(2, 1), _transformMatrix(2, 2));
        newRotation.z = std::atan2(_transformMatrix(1, 0), _transformMatrix(0, 0));
    }
	else
	{
        newRotation.x = std::atan2(-_transformMatrix(1, 2), _transformMatrix(1, 1));
        newRotation.z = 0;
    }
    _rotation = newRotation;
}

#pragma endregion