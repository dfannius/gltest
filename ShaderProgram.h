#pragma once

#include <GL3/gl3w.h>
#include <functional>
#include <vector>

#include "GLUtl.h"

class ShaderProgram
{
public:
   ShaderProgram();

   void Create();
   bool Created() const { return !mProgram.IsNull(); }

   void AttachShader( gl::shader_id shader );

   void Activate();

   void Link();
   void Validate();
   void Use();
   void CleanupShaders();

   gl::program_id ProgramID() const { return mProgram; }
   
private:
   void PerformAction( std::function< void( gl::program_id ) > gl_func,
                       GLenum status_type,
                       const char* desc );

   gl::program_id mProgram;
   std::vector< gl::shader_id > mShaders;
};

