#include <iostream>

#include "Camera.h"
#include "TransformState.h"

Camera::Camera():
   mTransform( new TransformState )
{
}

Camera::~Camera()
{
   delete mTransform;
}

void Camera::SetPerspective( float left, float right,
                             float bottom, float top,
                             float z_near, float z_far )
{
   mView.SetPerspective( left, right, bottom, top, z_near, z_far );
}

void Camera::LocalMove( const Vector3f& delta )
{
// std::cout << "Camera::LocalMove " << delta << std::endl;
// std::cout << "CURRENT STATE " << std::endl << *mTransform;
   Vector3f rotated_delta = (mTransform->GetRotationXfm() * Vector4f( delta )).ToVector3f();
// std::cout << "rotated delta " << rotated_delta << std::endl;
   mTransform->SetTranslation( mTransform->GetTranslation() + rotated_delta );
// std::cout << "NEW STATE " << std::endl << *mTransform;
}
