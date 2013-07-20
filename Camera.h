#pragma once

#include "Math.h"

class TransformState;

class Camera
{
public:
   Camera();
   ~Camera();

   void SetPerspective( float left, float right,
                        float bottom, float top,
                        float z_near, float z_far );

   void LocalMove( const Vector3f& delta );

   const Matrix4f& GetViewMtx() const { return mView; }

   TransformState& GetTransformState() { return *mTransform; }

   void PrintState();

private:
   TransformState* mTransform;
   Matrix4f mView;
};
