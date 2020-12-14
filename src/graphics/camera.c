#include "camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../math/math_utils.h"

void CameraComputeBasis(Camera* camera)
{
   camera->ForwardAxis.x = cos(DegToRad(camera->Yaw));
   camera->ForwardAxis.y = sin(DegToRad(camera->Pitch));
   camera->ForwardAxis.z = sin(DegToRad(camera->Yaw));

   camera->ForwardAxis = normalize_vector3(camera->ForwardAxis);

   camera->RightAxis = normalize_vector3(cross_vector3((Vector3) { 0.0f, 1.0f, 0.0f }, camera->ForwardAxis));
   camera->UpAxis = cross_vector3(camera->ForwardAxis, camera->RightAxis);
}

Camera* CreateCamera(const Vector3 position, const float fov, const float aspect, const float speed)
{
   Camera* newCamera = malloc(sizeof(Camera));
   if (!newCamera)
      return NULL;

   newCamera->Position = position;

   newCamera->Fov    = fov;
   newCamera->Aspect = aspect;
   newCamera->Speed  = speed;

   newCamera->Yaw   = -90.0f;
   newCamera->Pitch = 0.0f;

   CameraComputeBasis(newCamera);

   return newCamera;
}

void MoveCamera(Camera* camera, const CameraMoveType moveType)
{
   switch (moveType)
   {
   case CameraMoveForward:  camera->Position = add_vector3(camera->Position, muliply_svector3(camera->ForwardAxis, camera->Speed)); break;
   case CameraMoveBackward: camera->Position = subtract_vector3(camera->Position, muliply_svector3(camera->ForwardAxis, camera->Speed)); break;
   case CameraMoveRight:    camera->Position = subtract_vector3(camera->Position, muliply_svector3(camera->RightAxis, camera->Speed)); break;
   case CameraMoveLeft:     camera->Position = add_vector3(camera->Position, muliply_svector3(camera->RightAxis, camera->Speed));  break;
   case CameraMoveUp:       camera->Position = subtract_vector3(camera->Position, muliply_svector3(camera->UpAxis, camera->Speed)); break;
   case CameraMoveDown:     camera->Position = add_vector3(camera->Position, muliply_svector3(camera->UpAxis, camera->Speed)); break;
   }
}

void RotateCamera(Camera* camera, const float posX, const float posY)
{
   static char init = 0;

   static float lastX = 0.0f;
   static float lastY = 0.0f;

   if (!init)
   {
      lastX = posX;
      lastY = posY;
      init = 1;
   }

   camera->Yaw += posX - lastX;
   camera->Pitch += posY - lastY;

   lastX = posX;
   lastY = posY;

   CameraComputeBasis(camera);
}

Matrix4 GetCameraViewMatrix(const Camera* camera)
{
   return CreateLookAtMatrix(camera->RightAxis, camera->UpAxis, camera->ForwardAxis, camera->Position);
}

Matrix4 GetCameraProjection(const Camera* camera)
{
   return CreatePerspectiveMatrix(camera->Aspect, camera->Fov, 0.01f, 1000.0f);
}