#include "TransformState.h"

TransformState::TransformState()
{
   mScale.SetOne();
   mTranslation.SetZero();
   mRotation.SetZero();

   mScaleXfm.SetScale( mScale );
   mTranslationXfm.SetTranslation( mTranslation );
   mRotationXfm.SetRotation( mRotation );

   ComputeXfm();
}

void TransformState::SetScale( const Vector3f& scale )
{
   mScale = scale;
   mScaleXfm.SetScale( mScale );
   ComputeXfm();
}

void TransformState::SetTranslation( const Vector3f& translation )
{
   mTranslation = translation;
   mTranslationXfm.SetTranslation( mTranslation );
   ComputeXfm();
}

void TransformState::SetRotation( const Vector3f& rotation )
{
   mRotation = rotation;
   mRotationXfm.SetRotation( mRotation );
   ComputeXfm();
}

void TransformState::ComputeXfm()
{
   mXfm = mTranslationXfm * mRotationXfm * mScaleXfm;
}
