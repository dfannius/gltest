#include <assert.h>
#include <cstdlib>
#include <iostream>
#include <string>

#include "GlUtl.h"
#include "Math.h"
#include "Mesh.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include "Utl.h"

// ---------------------------------------------------------------------

Renderer::Renderer():
   mVBO( -1 ),
   mIBO( -1 ),
   mVAO( -1 ),
   mScaleLocation( -1 ),
   mModelToCameraLocation( -1 ),
   mNormalModelToCameraLocation( -1 ),
   mCameraToClipLocation( -1 ),
   mLightPosLocation( -1 ),
   mScale( 0.f ),
   mLastMs( 0 ),
   mPipeline( new Pipeline )
{
   mLightPos.SetZero();

   InitGl();
   InitPipeline();
}

Renderer::~Renderer()
{
   for (auto mesh : mMeshes) {
      delete mesh;
   }
}

void Renderer::InitGl()
{
   InitGlVars();
   CompileShaders();
   CreateMeshes();
   CreateLight();
   CreateVertexBuffer();
   CreateIndexBuffer();
   InitializeVertexArrayObjects();
}

void Renderer::InitGlVars()
{
   glClearColor( 0.f, 0.f, 0.f, 0.f ); // Make black the clear color
   glEnable( GL_CULL_FACE );
   glCullFace( GL_BACK );
   glFrontFace( GL_CW );
   glProvokingVertex( GL_FIRST_VERTEX_CONVENTION );
}

void Renderer::InitPipeline()
{
   mPipeline->SetPerspective( -2.f, 2.f, -2.f, 2.f, 17.f, 53.f );
}

void Renderer::CreateMeshes()
{
   for (int i = 0; i < 1; ++i) {
      mMeshes.push_back( NewCube() );
   }
}

void Renderer::CreateLight()
{
   mLightPos = Vector3f( 0.f, 0.f, -5.f );
}

void Renderer::CreateVertexBuffer()
{
   // Create one Vertex Buffer Object
   gl::GenBuffers( 1, &mVBO );

   // Make it the active GL_ARRAY_BUFFER
   gl::BindBuffer( GL_ARRAY_BUFFER, mVBO );

   // Fill it with data
   // GL_STATIC_DRAW is a hint that this data is written once and drawn many times.
   glBufferData( GL_ARRAY_BUFFER,
                 mMeshes[0]->VertexDataSize(),
                 mMeshes[0]->VertexData(),
                 GL_STATIC_DRAW );

   gl::BindBuffer( GL_ARRAY_BUFFER, gl::null_buffer_id );
}

void Renderer::CreateIndexBuffer()
{
   gl::GenBuffers( 1, &mIBO );
   gl::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO );
   glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                 mMeshes[0]->IndexDataSize(),
                 mMeshes[0]->IndexData(),
                 GL_STATIC_DRAW );
   gl::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, gl::null_buffer_id );
}

void Renderer::InitializeVertexArrayObjects()
{
   gl::GenVertexArrays( 1, &mVAO ); // mVAO is now a VAO name
}

void Renderer::UpdateScene( long ms )
{
   const int delta_ms = ms - mLastMs;
   mScale += delta_ms / 1600.f;

   mLightPos.x = 0.f;
   mLightPos.y = 0.f;
   mLightPos.z = -22.f;
}

void Renderer::UpdateObjects( long ms )
{
   float sinf_mScale = sinf( mScale );
   float cosf_mScale = sinf( mScale );
   // float sinf_tenth_mScale = sinf( mScale * 0.1f );

   // float xs[] = { -1.5f, 1.5f };
   // float zs[] = { -20.f + sinf_mScale, -20.f - sinf_mScale };

   float xs[] = { sinf_mScale, -sinf_mScale };
   float ys[] = { 0.f, 0.f };
   float zs[] = { -20.f + cosf_mScale, -20.f };

   for (unsigned int i = 0; i < mMeshes.size(); ++i)
   {
      Mesh* mesh = mMeshes[i];

      Vector3f translation( xs[i], ys[i], zs[i] );
      mesh->SetTranslation( translation );

      //    Vector3f rot( mScale * 200.f, mScale * 200.f, mScale * 200.f );
      //    mesh->SetRotation( rot );
   }
}

void Renderer::SetUniforms( Mesh* mesh )
{
   mPipeline->SetTransformState( mesh->GetTransformState() );

   gl::UniformMatrix4fv( mModelToCameraLocation,
                         1,       // number of matrices
                         GL_TRUE, // we are row-major
                         mPipeline->GetModelToCameraXfm().Data() );

   const Matrix3f normal_to_camera_mtx( mPipeline->GetModelToCameraXfm() );
   gl::UniformMatrix3fv( mNormalModelToCameraLocation,
                         1,
                         GL_TRUE,
                         normal_to_camera_mtx.Data() );

   gl::UniformMatrix4fv( mCameraToClipLocation,
                         1,
                         GL_TRUE,
                         mPipeline->GetCameraToClipXfm().Data() );

   gl::Uniform3fv( mLightPosLocation,
                   1,
                   mLightPos.v() );
}

void Renderer::DrawElements( Mesh* mesh )
{
   gl::BindVertexArray( mVAO );

   // Make mVBO the active GL_ARRAY_BUFFER, because
   // glVertexAttribPointer implicitly operates on GL_ARRAY_BUFFER
   gl::BindBuffer( GL_ARRAY_BUFFER, mVBO );

   // Enable the per-vertex inputs in the shader
   glEnableVertexAttribArray( 0 ); // position
   glEnableVertexAttribArray( 1 ); // normal
   glEnableVertexAttribArray( 2 ); // color

   // Specify how to access them
   mesh->glVertexAttribPointer( 0, Mesh::DataType::kVertexPos );
   mesh->glVertexAttribPointer( 1, Mesh::DataType::kVertexNormal );
   mesh->glVertexAttribPointer( 2, Mesh::DataType::kVertexColor );

   // Make mIBO the active GL_ELEMENT_ARRAY_BUFFER for use by glDrawElements
   gl::BindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIBO );

   glDrawElements( GL_TRIANGLES,           // mode
                   mesh->IndexDataCount(), // index count
                   GL_UNSIGNED_INT,        // type
                   0 );                    // first index
   gl::BindVertexArray( 0 );
}

void Renderer::Poll( long ms )
{
   LARGE_INTEGER frame_start_time;
   QueryPerformanceCounter( &frame_start_time );

   glClear( GL_COLOR_BUFFER_BIT ); // clear the region

   UpdateScene( ms );
   UpdateObjects( ms );

   for (auto mesh : mMeshes)
   {
      SetUniforms( mesh );
      DrawElements( mesh );
   }

   mLastMs = ms;

   LARGE_INTEGER frame_stop_time;
   QueryPerformanceCounter( &frame_stop_time );

   // LONGLONG frame_delta( frame_stop_time.QuadPart - frame_start_time.QuadPart );
   // DBOUT( frame_delta << "\n" );
}

void Renderer::CompileShaders()
{
   ShaderProgram program {};
   program.Create();

   LoadShader( program, "lighting.vert", GL_VERTEX_SHADER );
   LoadShader( program, "lighting.frag", GL_FRAGMENT_SHADER );

   program.Activate();

   gl::program_id id = program.ProgramID();
   mModelToCameraLocation       = gl::GetUniformLocation( id, "model_to_camera_mtx" );
   mNormalModelToCameraLocation = gl::GetUniformLocation( id, "normal_to_camera_mtx" );
   mCameraToClipLocation        = gl::GetUniformLocation( id, "camera_to_clip_mtx" );
   mLightPosLocation            = gl::GetUniformLocation( id, "light_pos" );
}

void Renderer::LoadShader( ShaderProgram& shader_program,
                           const char* filename,
                           GLenum shader_type )
{
   std::string src;
   ReadFileToString( filename, src );
   AddShader( shader_program, src.c_str(), filename, shader_type );
}

void Renderer::AddShader( ShaderProgram& shader_program,   // ID of the program
                          const char* shader_text, // source text
                          const char* shader_name, // file name
                          GLenum shader_type )     // vertex or fragment
{
   gl::shader_id shader_obj = gl::CreateShader( shader_type );

   if (shader_obj.val == 0) {
      DBOUT( "Error creating " << shader_name << "\n" );
      exit( 0 );
   }

   const GLchar* strings[1] { shader_text };
   GLint lengths[1] { GLint( strlen( shader_text ) ) };

   gl::ShaderSource( shader_obj,  // shader to set source code for
                     1,           // size of p and lengths
                     strings,     // array of pointers to strings with shader code
                     lengths );   // array of lengths of those strings
   gl::CompileShader( shader_obj );

   GLint success;
   gl::GetShaderiv( shader_obj,
                    GL_COMPILE_STATUS, // parameter to get
                    &success);         // parameter is stuffed here

   if (!success) {
      GLchar InfoLog[1024];
      gl::GetShaderInfoLog( shader_obj, 1024, NULL, InfoLog );
      DBOUT( "Error compiling " << shader_name << ": '" << InfoLog << "\n" );
      exit( 1 );
   }

   shader_program.AttachShader( shader_obj );
}
