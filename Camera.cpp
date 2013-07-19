#include "Camera.h"
#include "TransformState.h"

Camera::Camera():
   mTransformState( new TransformState )
{
}

Camera::~Camera()
{
   delete mTransformState;
}
