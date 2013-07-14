#ifndef GLTEST_PIPELINE_H
#define GLTEST_PIPELINE_H

#include "Math.h"

class TransformState;

class Pipeline
{
public:
   Pipeline():
      mScale( 1.f, 1.f, 1.f ),
      mTranslation( 0.f, 0.f, 0.f ),
      mRotation( 0.f, 0.f, 0.f ),
      mDirty( false )
   {
   }

   const Matrix4f& GetModelToCameraXfm();
   const Matrix4f& GetCameraToClipXfm();

   void SetScale( const Vector3f& scale )
   {
      mScale = scale;
      InitScaleXfm();
   }

   void SetScale( float sx, float sy, float sz )
   {
      mScale.x = sx;
      mScale.y = sy;
      mScale.z = sz;
      InitScaleXfm();
   }

   void SetTranslation( const Vector3f& world_pos )
   {
      mTranslation = world_pos;
      InitTranslationXfm();
   }

   void SetTranslation( float x, float y, float z )
   {
      mTranslation.x = x;
      mTranslation.y = y;
      mTranslation.z = z;
      InitTranslationXfm();
   }

   void SetRotation( const Vector3f& rotation )
   {
      mRotation = rotation;
      InitRotationXfm();
   }

   // Euler angles
   void SetRotation( float rot_x, float rot_y, float rot_z )
   {
      mRotation.x = rot_x;
      mRotation.y = rot_y;
      mRotation.z = rot_z;
      InitRotationXfm();
   }

   void SetPerspective( float left, float right,
                        float bottom, float top,
                        float z_near, float z_far )
   {
      mPerspVars.left = left;
      mPerspVars.right = right;
      mPerspVars.bottom = bottom;
      mPerspVars.top = top;
      mPerspVars.z_near = z_near;
      mPerspVars.z_far = z_far;
      InitPerspective();
    }

   void SetTransformState( TransformState& state );

   void DebugVector( const Vector3f& v ) const;

private:
   void InitScaleXfm();
   void InitRotationXfm();
   void InitTranslationXfm();
   void InitPerspective();

   Matrix4f const & ComputeXfm() const; // set mXfm if necessary

   Vector3f mScale;
   Vector3f mTranslation;
   Vector3f mRotation;

   struct {
      float left;
      float right;
      float bottom;
      float top;
      float z_near;
      float z_far;
   } mPerspVars;

   Matrix4f mScaleXfm;
   Matrix4f mRotationXfm;
   Matrix4f mTranslationXfm;
   Matrix4f mPerspectiveXfm;

   mutable Matrix4f mXfm;
   mutable bool mDirty;
};

#endif
