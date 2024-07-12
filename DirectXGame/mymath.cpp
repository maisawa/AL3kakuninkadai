#include "myMath.h"
#include <numbers>
#include <cmath>

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

    Matrix4x4 result = Multiply(
        Multiply(MakeRotateXMatrix(rotate.x), MakeRotateYMatrix(rotate.y)),
        MakeRotateZMatrix(rotate.z));

    result.m[0][0] *= scale.x;
    result.m[0][1] *= scale.x;
    result.m[0][2] *= scale.x;
    result.m[1][0] *= scale.y;
    result.m[1][1] *= scale.y;
    result.m[1][2] *= scale.y;
    result.m[2][0] *= scale.z;
    result.m[2][1] *= scale.z;
    result.m[2][2] *= scale.z;
    result.m[3][0] = translate.x;
    result.m[3][1] = translate.y;
    result.m[3][2] = translate.z;

    return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    return { 1.0f, 0.0f, 0.0f,0.0f,
                0.0f, cosTheta, sinTheta, 0.0f,
                0.0f, -sinTheta, cosTheta, 0.0f,
                0.0f, 0.0f,0.0f,1.0f };
}

Matrix4x4 MakeRotateYMatrix(float radian) {
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    return { cosTheta, 0.0f, -sinTheta, 0.0f,
                 0.0f, 1.0f, 0.0f, 0.0f,
                sinTheta, 0.0f, cosTheta,  0.0f,
                0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix4x4 MakeRotateZMatrix(float radian) {


    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    return { cosTheta, sinTheta, 0.0f, 0.0f,                 -sinTheta,cosTheta, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f,

                  0.0f, 0.0f, 0.0f, 1.0f };
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