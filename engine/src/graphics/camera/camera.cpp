#include "camera.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "math/math_utils.h"

namespace graphics
{
   void CameraComputeBasis(Camera& camera)
   {
      camera.ForwardAxis.x = cos(math::ToRad(camera.Yaw)) * cos(math::ToRad(camera.Pitch));
      camera.ForwardAxis.y = sin(math::ToRad(camera.Pitch));
      camera.ForwardAxis.z = sin(math::ToRad(camera.Yaw)) * cos(math::ToRad(camera.Pitch));

      camera.ForwardAxis = mm::normalize(camera.ForwardAxis);

      camera.RightAxis = mm::normalize(mm::cross({ 0.0f, 1.0f, 0.0f }, camera.ForwardAxis));
      camera.UpAxis = mm::cross(camera.ForwardAxis, camera.RightAxis);
   }

   Camera::Camera(const mm::vec3 position, const float fov, const float aspect, const float speed)
   {
      Position = position;

      Fov = fov;
      Aspect = aspect;
      Speed = speed;

      Yaw = 90.0f;
      Pitch = 0.0f;

      CameraComputeBasis(*this);
   }

   void Camera::Move(const CameraMoveType moveType, const float value, const float deltaTime)
   {
      float speed = Speed * value * deltaTime;

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