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
