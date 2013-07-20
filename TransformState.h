#pragma once

#include "Math.h"

class TransformState
{
public:
   TransformState();

   void SetScale( const Vector3f& scale );
   void SetTranslation( const Vector3f& translation );
   void SetRotation( const Vector3f& rotation );

   const Vector3f& GetScale() { return mScale; }
   const Vector3f& GetTranslation() { return mTranslation; }
   const Vector3f& GetRotation() { return mRotation; }

   const Matrix4f& GetRotationXfm() { return mRotationXfm; }
   const Matrix4f& GetXfm() { return mXfm; }

public:
   void ComputeXfm();

   Vector3f mScale;
   Vector3f mTranslation;
   Vector3f mRotation;

   Matrix4f mScaleXfm;
   Matrix4f mTranslationXfm;
   Matrix4f mRotationXfm;

   Matrix4f mXfm;               // composition
};

std::ostream& operator<< ( std::ostream &out, const TransformState &xfm );
