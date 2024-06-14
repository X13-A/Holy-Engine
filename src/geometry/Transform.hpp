#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../math/Vec3.hpp"
#include "../math/Mat4.hpp"
#include <memory>

class Mesh;

class Transform
{
private:
	Mat4 _transformMatrix;
	Vec3 _position;
	Vec3 _rotation;
	Vec3 _scale;

public:
	Transform(const Vec3& position, const Vec3& rotation, const Vec3& scale);

	const Mat4& getTransformMatrix() const;
	const Vec3& getPosition() const;
	const Vec3& getRotation() const;
	const Vec3& getScale() const;

	void setScale(const Vec3& value);
	void setPosition(const Vec3& newPosition);
	void setRotation(const Vec3& newRotation);

	void translate(const Vec3& offset);
	void rotate(const Vec3& degrees);
	void scale(const Vec3& value);
};


#endif