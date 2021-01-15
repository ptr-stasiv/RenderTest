#pragma once
#include "../math/vectors/vector3.h"
#include "../math/matrices/matrix4x4.h"

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
      math::Vector3 Position;

      math::Vector3 ForwardAxis;
      math::Vector3 RightAxis;
      math::Vector3 UpAxis;

      float Fov;
      float Aspect;
      float Speed;

      float Yaw;
      float Pitch;

      Camera(const math::Vector3 position, const float fov, const float aspect, const float speed);

      Camera()  = default;
      ~Camera() = default;

      void Move(const CameraMoveType moveType, const float deltaTime);
      void Rotate(const float posX, const float posY, const float deltaTime);

      inline math::Matrix4 GetCameraViewMatrix() const
      {
         return math::CreateLookAtMatrix(RightAxis, UpAxis, ForwardAxis, Position);
      }

      inline math::Matrix4 GetCameraProjection() const
      {
         return math::CreatePerspectiveMatrix(Aspect, Fov, 0.01f, 1000.0f);
      }
   };
}