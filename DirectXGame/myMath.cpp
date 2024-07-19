#include "myMath.h"
#include "GameScene.h"
#include <cassert>
#include <numbers>
using namespace std;

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	 Matrix4x4 resultAffine = Multiply(
        Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)),
        MakeRotateZMatrix(rotate.z));

    resultAffine.m[0][0] *= scale.x;
    resultAffine.m[0][1] *= scale.x;
    resultAffine.m[0][2] *= scale.x;
    resultAffine.m[1][0] *= scale.y;
    resultAffine.m[1][1] *= scale.y;
    resultAffine.m[1][2] *= scale.y;
    resultAffine.m[2][0] *= scale.z;
    resultAffine.m[2][1] *= scale.z;
    resultAffine.m[2][2] *= scale.z;
    resultAffine.m[3][0] = translate.x;
    resultAffine.m[3][1] = translate.y;
    resultAffine.m[3][2] = translate.z;

    return resultAffine;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
    float cosTheta = cos(radian);
    float sinTheta = sin(radian);
    return {1.0f, 0.0f,      0.0f,     0.0f, 0.0f, cosTheta, sinTheta, 0.0f,
            0.0f, -sinTheta, cosTheta, 0.0f, 0.0f, 0.0f,     0.0f,     1.0f};
}
Matrix4x4 MakeRotateYMatrix(float radian) {
    float cosTheta = cos(radian);
    float sinTheta = sin(radian);
    return {cosTheta, 0.0f, -sinTheta, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            sinTheta, 0.0f, cosTheta,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}
Matrix4x4 MakeRotateZMatrix(float radian) {
    float cosTheta = cos(radian);
	float sinTheta = sin(radian);
	return {cosTheta, sinTheta, 0.0f, 0.0f, -sinTheta, cosTheta, 0.0f, 0.0f,
		    0.0f,     0.0f,     1.0f, 0.0f, 0.0f,      0.0f,     0.0f, 1.0f};
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
   
     Matrix4x4 result3;
    for (int m = 0; m < 4; m++) {
        for (int n = 0; n < 4; n++) {
            result3.m[m][n] = 0;
            for (int mn = 0; mn < 4; mn++) {
                result3.m[m][n] += m1.m[m][mn] * m2.m[mn][n];
            }
        }
    }
    return result3;
}

Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}

Vector3& operator-=(Vector3& lhv, const Vector3& rhv) {
	lhv.x -= rhv.x;
	lhv.y -= rhv.y;
	lhv.z -= rhv.z;
	return lhv;
}

Vector3& operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

Vector3& operator/=(Vector3& v, float s) {
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v) { 
	return v * s; 
}

const Vector3 operator/(const Vector3& v, float s) {
	Vector3 temp(v);
	return temp /= s;
}

float EaseInOut(float x1, float x2, float t) {
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

float Lerp(float x1, float x2, float t) { 
	return (1.0f - t) * x1 + t * x2; 
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { 
	return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); 
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}