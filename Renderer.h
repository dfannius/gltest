#pragma once

#include <GL3/gl3w.h>

#include <functional>
#include <vector>

#include "GlUtl.h"
#include "Math.h"

class Mesh;
class Pipeline;

class Renderer
{
public:
   Renderer();
   ~Renderer();

   void Poll( long ms );

private:
   void InitGl();
   void InitGlVars();
   void InitPipeline();

   void CreateMeshes();
   void CreateLight();

   void CreateVertexBuffer();
   void CreateIndexBuffer();
   void InitializeVertexArrayObjects();

   void SetUniforms( Mesh* mesh );
   void DrawElements( Mesh* mesh );

   void LoadShader( gl::program_id shader_program, const char* filename, GLenum shader_type );
   void CompileShaders();
   void LinkProgram( gl::program_id shader_program );
   void ValidateProgram( gl::program_id shader_program );
   void PerformProgramAction( gl::program_id shader_program,
                              std::function< void( gl::program_id ) > gl_func,
                              GLenum status_type,
                              const char* desc );
   void AddShader( gl::program_id shader_program,
                   const char* shader_text,
                   const char* shader_name,
                   GLenum shader_type );

   void UpdateScene( long ms );
   void UpdateObjects( long ms );

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

   Pipeline* mPipeline;

   std::vector<Mesh*> mMeshes;
   Vector3f mLightPos;
};
