#include "myMath.h"

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