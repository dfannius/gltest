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

std::ostream& operator<< ( std::ostream &out, const TransformState &xfm )
{
   out << "Scale " << xfm.mScale << std::endl;
   out << "Trans " << xfm.mTranslation << std::endl;
   out << "Rotat " << xfm.mRotation << std::endl;

   out << "Scale" << std::endl << xfm.mScaleXfm;
   out << "Translation" << std::endl << xfm.mTranslationXfm;
   out << "Rotation" << std::endl << xfm.mRotationXfm;

   out << "Composition" << std::endl << xfm.mXfm;

   return out;
}
