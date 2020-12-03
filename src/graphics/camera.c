#include "camera.h"

#include <stdio.h>
#include <stdlib.h>

Camera* CreateCamera(const Vector3 position, const float fov, const float aspect, const float speed)
{
   Camera* newCamera = malloc(sizeof(Camera));
   if (!newCamera)
      return NULL;

   newCamera->Position = position;

   newCamera->ForwardAxis = (Vector3){ 0.0f, 0.0f, 1.0f };
   newCamera->RightAxis   = normalize_vector3(cross_vector3((Vector3) { 0.0f, 1.0f, 0.0f }, newCamera->ForwardAxis));
   newCamera->UpAxis      = cross_vector3(newCamera->ForwardAxis, newCamera->RightAxis);

   newCamera->Fov    = fov;
   newCamera->Aspect = aspect;
   newCamera->Speed  = speed;

   newCamera->Yaw   = 0.0f;
   newCamera->Pitch = 0.0f;
}

void MoveCamera(Camera* camera, const CameraMoveType moveType)
{
   switch (moveType)
   {
   case CameraMoveForward: camera->Position = add_vector3(camera->Position, muliply_svector3(camera->ForwardAxis, camera->Speed)); break;
   case CameraMoveBackward: camera->Position = subtract_vector3(camera->Position, muliply_svector3(camera->ForwardAxis, camera->Speed)); break;
   case CameraMoveRight: camera->Position = add_vector3(camera->Position, muliply_svector3(camera->RightAxis, camera->Speed)); break;
   case CameraMoveLeft: camera->Position = subtract_vector3(camera->Position, muliply_svector3(camera->RightAxis, camera->Speed));  break;
   case CameraMoveUp: camera->Position = add_vector3(camera->Position, muliply_svector3(camera->UpAxis, camera->Speed)); break;
   case CameraMoveDown: camera->Position = subtract_vector3(camera->Position, muliply_svector3(camera->UpAxis, camera->Speed)); break;
   }
}

Matrix4 GetCameraViewMatrix(const Camera* camera)
{
   return CreateTranslateMatrix(muliply_svector3(camera->Position, -1.0f));
}

Matrix4 GetCameraProjection(const Camera* camera)
{
   return CreatePerspectiveMatrix(camera->Aspect, camera->Fov, 0.1f, 1000.0f);
}