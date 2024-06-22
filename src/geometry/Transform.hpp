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
	Vec3 getTransformMatrixPosition() const;
	const Vec3 &getRotation() const;
	const Vec3& getScale() const;
	float getYaw() const;
	float getPitch() const;
	float getRoll() const;

	void setScale(const Vec3& value);
	void setPosition(const Vec3& newPosition);
	void setRotation(const Vec3& newRotation);

	void lookat(const Vec3 &target, const Vec3 &up);
	void translate(const Vec3 &offset);
	void translateLocal(const Vec3& offset);
	void rotate(const Vec3& degrees);
	void scale(const Vec3& value);
	void printValues();
};


#endif