#pragma once
#include "../math/vectors/vector3.h"
#include "../math/matrices/matrix4x4.h"

typedef struct CameraStructure
{
   Vector3 Position;

   Vector3 ForwardAxis;
   Vector3 RightAxis;
   Vector3 UpAxis;

   float Fov;
   float Aspect;
   float Speed;

   float Yaw;
   float Pitch;
} Camera;

typedef enum CameraMoveTypeEnum
{
   CameraMoveForward,
   CameraMoveBackward,
   CameraMoveRight,
   CameraMoveLeft,
   CameraMoveUp,
   CameraMoveDown
} CameraMoveType;

Camera* CreateCamera(const Vector3 position, const float fov, const float aspect, const float speed);

void MoveCamera(Camera* camera, const CameraMoveType moveType);
void RotateCamera(Camera* camera, const float posX, const float posY);

Matrix4 GetCameraViewMatrix(const Camera* camera);
Matrix4 GetCameraProjection(const Camera* camera);