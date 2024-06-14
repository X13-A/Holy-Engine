#include "Transform.hpp"
#include <vector>
#include "../math/Math.hpp"
#include <iostream>

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

const float& Transform::getYaw() const
{
    return atan2(_transformMatrix(2, 0), _transformMatrix(0, 0));
}

const float& Transform::getPitch() const
{
    return asin(-_transformMatrix(1, 0));
}

const float& Transform::getRoll() const
{
    return atan2(_transformMatrix(2, 1), _transformMatrix(2, 2));
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

void Transform::translateLocal(const Vec3& offset)
{
	// Vec3 localOffset;
	// localOffset.x = right * offset.x;
	// localOffset.y = up * offset.y;
	// localOffset.z = forward * offset.z;

	// _position += localOffset;
	// _transformMatrix.translate(localOffset);
}

void Transform::rotate(const Vec3& degrees)
{
	Vec3 rads = radians(degrees);
	_rotation += rads;
	std::cout << degrees.y << ": " << rads.y << ": " << _rotation.y << std::endl;

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

	// Prevent lookat from affecting position
	_transformMatrix(0, 3) = _position.x;
	_transformMatrix(1, 3) = _position.y;
	_transformMatrix(2, 3) = _position.z;
	_transformMatrix(3, 3) = 1.0;

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