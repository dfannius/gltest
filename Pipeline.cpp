#include <sstream>

#include "Pipeline.h"
#include "TransformState.h"
#include "Utl.h"

void Pipeline::DebugVector( Vector3f const& v ) const
{
   ComputeXfm();
   Vector4f vout = mXfm * v;
   DBOUT( "Projection of " << v << " = " << vout << "\n" );
}

void Pipeline::InitScaleXfm()
{
   mScaleXfm.SetZero();
   mScaleXfm( 0, 0 ) = mScale.x;
   mScaleXfm( 1, 1 ) = mScale.y;
   mScaleXfm( 2, 2 ) = mScale.z;
   mScaleXfm( 3, 3 ) = 1.f;
   mDirty = true;
}

void Pipeline::InitRotationXfm()
{
   Matrix4f rx, ry, rz;

   const float x = degrees_to_radians( mRotation.x );
   const float y = degrees_to_radians( mRotation.y );
   const float z = degrees_to_radians( mRotation.z );

   float cosfx = cosf( x ); float sinfx = sinf( x );
   float cosfy = cosf( y ); float sinfy = sinf( y );
   float cosfz = cosf( z ); float sinfz = sinf( z );

   rx.SetIdentity();
   rx( 1, 1 ) = cosfx; rx( 1, 2 ) = -sinfx;
   rx( 2, 1 ) = sinfx; rx( 2, 2 ) = cosfx;

   ry.SetIdentity();
   ry( 0, 0 ) = cosfy; ry( 0, 2 ) = -sinfy;
   ry( 2, 0 ) = sinfy; ry( 2, 2 ) = cosfy;

   rz.SetIdentity();
   rz( 0, 0 ) = cosfz; rz( 0, 1 ) = -sinfz;
   rz( 1, 0 ) = sinfz; rz( 1, 1 ) = cosfz;

   mRotationXfm = rx * ry * rz;
   mDirty = true;
}

void Pipeline::InitTranslationXfm()
{
   mTranslationXfm.SetIdentity();
   mTranslationXfm( 0, 3 ) = mTranslation.x;
   mTranslationXfm( 1, 3 ) = mTranslation.y;
   mTranslationXfm( 2, 3 ) = mTranslation.z;
   mDirty = true;
}

void Pipeline::InitPerspective()
{
   const float left   = mPerspVars.left;
   const float right  = mPerspVars.right;
   const float bottom = mPerspVars.bottom;
   const float top    = mPerspVars.top;
   const float z_near = mPerspVars.z_near;
   const float z_far  = mPerspVars.z_far;

   mPerspectiveXfm.SetZero();

   mPerspectiveXfm.m[0][0] = 2.f * z_near / (right - left);
   mPerspectiveXfm.m[0][2] = (right + left) / (right - left);
   mPerspectiveXfm.m[1][1] = 2.f * z_near / (top - bottom);
   mPerspectiveXfm.m[1][2] = (top + bottom) / (top - bottom);
   mPerspectiveXfm.m[2][2] = - (z_far + z_near) / (z_far - z_near);
   mPerspectiveXfm.m[2][3] = -2.f * z_far * z_near / (z_far - z_near );
   mPerspectiveXfm.m[3][2] = -1.f;

// DBOUT( "Perspective projection matrix :\n" << mPerspectiveXfm );

   mDirty = true;
}

const Matrix4f& Pipeline::ComputeXfm() const
{
   if (mDirty)
   {
      mXfm = mTranslationXfm * mRotationXfm * mScaleXfm;
      mDirty = false;
   }

   return mXfm;
}

const Matrix4f& Pipeline::GetModelToCameraXfm()
{
   return ComputeXfm();
}

const Matrix4f& Pipeline::GetCameraToClipXfm()
{
   return mPerspectiveXfm;
}

void Pipeline::SetTransformState( TransformState& state )
{
   SetScale( state.GetScale() );
   SetTranslation( state.GetTranslation() );
   SetRotation( state.GetRotation() );
}
