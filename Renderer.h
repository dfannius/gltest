#pragma once

#include <GL3/gl3w.h>

#include <functional>
#include <vector>

#include "GlUtl.h"
#include "Math.h"
#include "Movement.h"

class Camera;
class Mesh;
class ShaderProgram;

class Renderer
{
public:
   Renderer();
   ~Renderer();

   void Poll( long ms );

   void Debug() { mDebugFrame = true; }
   void SetMovementFlag( MovementFlag flag ) { mMovementFlags.SetFlag( flag ); }
   void ClearMovementFlag( MovementFlag flag ) { mMovementFlags.ClearFlag( flag ); }

private:
   void InitGl();
   void InitGlVars();
   void InitCamera();

   void CreateMeshes();
   void CreateLight();

   void CreateVertexBuffer();
   void CreateIndexBuffer();
   void InitializeVertexArrayObjects();

   void SetUniforms( Mesh* mesh );
   void DrawElements( Mesh* mesh );

   void AssignLocation( gl::location_id& loc, gl::program_id prog, const char* name );
   void CompileShaders();
   void LoadShader( ShaderProgram& shader_program, const char* filename, GLenum shader_type );
   void AddShader( ShaderProgram& shader_program,
                   const char* shader_text,
                   const char* shader_name,
                   GLenum shader_type );

   void UpdateScene( long ms );
   void UpdateObjects( long ms );
   void UpdateCamera( long ms );

   bool mDebugFrame = false;

   gl::buffer_id mVBO;       // vertex buffer object
   gl::buffer_id mIBO;       // index buffer object
   gl::vertex_array_id mVAO; // vertex array object

   gl::location_id mScaleLocation;
   gl::location_id mModelToCameraLocation;
   gl::location_id mNormalModelToCameraLocation;
   gl::location_id mCameraToClipLocation;
   gl::location_id mLightPosLocation;

   float mScale;

   long mLastMs;

   Camera* mCamera;

   std::vector<Mesh*> mMeshes;
   Vector3f mLightPos;

   MovementFlags mMovementFlags;
};
