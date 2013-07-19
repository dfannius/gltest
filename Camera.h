#pragma once

class TransformState;

class Camera
{
public:
   Camera();
   ~Camera();

private:
   TransformState* mTransformState;
};
