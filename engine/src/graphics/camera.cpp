#include "camera.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "math/math_utils.h"

namespace graphics
{
   void CameraComputeBasis(Camera& camera)
   {
      camera.ForwardAxis.x = cos(DegToRad(camera.Yaw)) * cos(DegToRad(camera.Pitch));
      camera.ForwardAxis.y = sin(DegToRad(camera.Pitch));
      camera.ForwardAxis.z = sin(DegToRad(camera.Yaw)) * cos(DegToRad(camera.Pitch));

      camera.ForwardAxis = math::Normalize(camera.ForwardAxis);

      camera.RightAxis = math::Normalize(math::Cross({ 0.0f, 1.0f, 0.0f }, camera.ForwardAxis));
      camera.UpAxis = math::Cross(camera.ForwardAxis, camera.RightAxis);
   }

   Camera::Camera(const math::Vector3 position, const float fov, const float aspect, const float speed)
   {
      Position = position;

      Fov = fov;
      Aspect = aspect;
      Speed = speed;

      Yaw = 90.0f;
      Pitch = 0.0f;

      CameraComputeBasis(*this);
   }

   void Camera::Move(const CameraMoveType moveType, const float deltaTime)
   {
      float speed = Speed * deltaTime;

      switch (moveType)
      {
      case CameraMoveType::MoveForward:  Position -= ForwardAxis * speed; break;
      case CameraMoveType::MoveBackward: Position += ForwardAxis * speed; break;
      case CameraMoveType::MoveRight:    Position += RightAxis * speed; break;
      case CameraMoveType::MoveLeft:     Position -= RightAxis * speed;  break;
      case CameraMoveType::MoveUp:       Position += UpAxis * speed; break;
      case CameraMoveType::MoveDown:     Position -= UpAxis * speed; break;
      }
   }

   void Camera::Rotate(const float posX, const float posY, const float deltaTime)
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

      Yaw += (posX - lastX) * deltaTime * 7.0f;
      Pitch += (posY - lastY) * deltaTime * 7.0f;

      lastX = posX;
      lastY = posY;

      CameraComputeBasis(*this);
   }
}