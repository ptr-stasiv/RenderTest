#pragma once
#include "math/math.h"

namespace graphics
{
   enum class CameraMoveType
   {
      MoveForward,
      MoveBackward,
      MoveRight,
      MoveLeft,
      MoveUp,
      MoveDown
   };

   struct Camera
   {
      mm::vec3 Position;

      mm::vec3 ForwardAxis;
      mm::vec3 RightAxis;
      mm::vec3 UpAxis;

      float Fov;
      float Aspect;
      float Speed;

      float Yaw;
      float Pitch;

      Camera(const mm::vec3 position, const float fov, const float aspect, const float speed);

      Camera()  = default;
      ~Camera() = default;

      void Move(const CameraMoveType moveType, const float value, const float deltaTime);
      void Rotate(const float posX, const float posY, const float deltaTime);

      inline mm::mat4 GetCameraViewMatrix() const
      {
         return mm::lookAt(RightAxis, UpAxis, ForwardAxis, Position);
      }

      inline mm::mat4 GetCameraProjection() const
      {
         return mm::perspective(Fov, Aspect, 0.01f, 1000.0f);
      }
   };
}