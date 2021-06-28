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

   //TODO rework this class, reason is a lot of C code
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

      float Size;
      float Near;
      float Far;

      bool Perspective;

      Camera(const mm::vec3& position, const float fov, const float aspect, const float speed, const float yaw = 90.0f, const float pitch = 0.0f);
      Camera(const mm::vec3& position, const mm::vec3& direction, const float fov, const float aspect, const float speed);

      Camera(const float size, const float far, const float near, const mm::vec3& position);
      Camera(const float size, const float far, const float near, const mm::vec3& position, const mm::vec3& direction);

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
         if(Perspective)
            return mm::perspective(Fov, Aspect, 0.01f, 1000.0f);
         else
            return mm::ortho(Size, Size, Far, Near);
      }
   };
}