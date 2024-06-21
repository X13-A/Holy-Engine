#include "Transform.hpp"
#include <vector>
#include "../math/Math.hpp"
#include <iostream>

Transform::Transform(const Vec3& position, const Vec3& rotation, const Vec3& scale) : _transformMatrix(Mat4())
{
	setPosition(position);
	setScale(scale);
	setRotation(rotation);
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

// Used for debug, because it does not match position
Vec3 Transform::getTransformMatrixPosition() const
{
	Vec3 pos;
	pos.x = getTransformMatrix()(0, 3);
	pos.y = getTransformMatrix()(1, 3);
	pos.z = getTransformMatrix()(2, 3);
	return pos;
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

void Transform::setPitchYaw(float pitch, float yaw)
{
    setRotation(Vec3(pitch, yaw, 0.0));
}

void Transform::lookat(const Vec3& target, const Vec3& up)
{
    // TODO
    std::cerr << "lookAt not implemented" << std::endl;
    exit(1);
}

// TODO: Fix as it's broken.
void Transform::setRotation(const Vec3& newRotation)
{
    // Reset the transform matrix to identity
    _transformMatrix = Mat4();

    // Apply scale
    _transformMatrix.scale(_scale);

    // Create rotation matrices for each axis
    Mat4 rotationX = Mat4::rotation_x(newRotation.x);
    Mat4 rotationY = Mat4::rotation_y(newRotation.y);
    Mat4 rotationZ = Mat4::rotation_z(newRotation.z);

    // Combine the rotations: rotationZ * rotationY * rotationX
    Mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    _transformMatrix = _transformMatrix * rotationMatrix;

    // Apply translation
    _transformMatrix.translate(_position);

    // Update the rotation
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
    // Extract the local coordinate axes from the rotation matrix
    Vec3 right(_transformMatrix(0, 0), _transformMatrix(1, 0), _transformMatrix(2, 0));
    Vec3 up(_transformMatrix(0, 1), _transformMatrix(1, 1), _transformMatrix(2, 1));
    Vec3 forward(_transformMatrix(0, 2), _transformMatrix(1, 2), _transformMatrix(2, 2));

    // Compute the local offset
    Vec3 localOffset = right * offset.x + up * offset.y + forward * offset.z;

    // Update the position and the transformation matrix
    _position += localOffset;
	std::cout << localOffset.length() << std::endl;
	_transformMatrix.translate(localOffset);
}

void Transform::rotate(const Vec3& degrees)
{
	Vec3 rads = radians(degrees);
	_rotation += rads;
	_transformMatrix.rotate(rads, getPosition());
}

void Transform::scale(const Vec3& value)
{
	_scale *= value;
	_transformMatrix.scale(value);
}

// Print camera values
void Transform::printValues()
{
    std::cout << "Pos: " << getPosition().x << ", " << getPosition().y << ", " << getPosition().z << std::endl;
    std::cout << "Forward: " << getRotation().x << ", " << getRotation().y << ", " << getRotation().z << std::endl;
}

#pragma endregion