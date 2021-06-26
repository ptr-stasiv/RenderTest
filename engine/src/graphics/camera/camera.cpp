#include "camera.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "math/math.h"

namespace graphics
{
   void CameraComputeBasis(Camera& camera)
   {
      camera.ForwardAxis.x = cos(mm::ToRad(camera.Yaw)) * cos(mm::ToRad(camera.Pitch));
      camera.ForwardAxis.y = sin(mm::ToRad(camera.Pitch));
      camera.ForwardAxis.z = sin(mm::ToRad(camera.Yaw)) * cos(mm::ToRad(camera.Pitch));

      camera.ForwardAxis = mm::normalize(camera.ForwardAxis);

      camera.RightAxis = mm::normalize(mm::cross({ 0.0f, 1.0f, 0.0f }, camera.ForwardAxis));
      camera.UpAxis = mm::cross(camera.ForwardAxis, camera.RightAxis);
   }

   Camera::Camera(const mm::vec3& position, const bool perspective, const float fov, const float aspect, const float speed, const float yaw, const float pitch)
   {
      Position = position;

      Fov = fov;
      Aspect = aspect;
      Speed = speed;

      Yaw = yaw;
      Pitch = pitch;

      Perspective = perspective;

      CameraComputeBasis(*this);
   }

   Camera::Camera(const mm::vec3& position, const bool perspective, const float fov, const float aspect, const float speed, const mm::vec3& direction)
   {
      Position = position;

      Fov = fov;
      Aspect = aspect;
      Speed = speed;

      Perspective = perspective;

      ForwardAxis = direction;

      UpAxis = ForwardAxis;
      UpAxis.x += 1.0f;
      UpAxis = mm::normalize(UpAxis);

      RightAxis = mm::normalize(mm::cross(UpAxis, ForwardAxis));
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