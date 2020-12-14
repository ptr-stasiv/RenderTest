#pragma once
#include "../vectors/vector4.h"
#include "../vectors/vector3.h"

typedef struct Matrix4Structure
{
   float Data[16];
}Matrix4;

Matrix4 CreateIdentityMatrix4();
Matrix4 CreatePerspectiveMatrix(const float aspect, const float fov, const float near, const float far);
Matrix4 CreateTranslateMatrix(const Vector3 offset);
Matrix4 CreateScaleMatrix(const Vector3 scale);
Matrix4 CreateRotationMatrix(const float a, const Vector3 axis);
Matrix4 CreateLookAtMatrix(const Vector3 axisX, const Vector3 axisY, const Vector3 axisZ, const Vector3 offset);

Vector4 multiply_vmatrix4(const Vector4 v, const Matrix4 m);

Matrix4 multiply_matrix(const Matrix4 m1, const Matrix4 m2);