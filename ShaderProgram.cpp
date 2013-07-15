#include <assert.h>
#include <cstdlib>
#include <sstream>

#include "ShaderProgram.h"
#include "Utl.h"

ShaderProgram::ShaderProgram():
   mProgram( gl::program_id( 0 ) )
{
}

void ShaderProgram::Create()
{
   assert( !Created() );
   mProgram = gl::CreateProgram();

   if (!Created()) {
      DBOUT( "Error creating shader program\n" );
      exit( 1 );
   }

}

void ShaderProgram::AttachShader( gl::shader_id shader )
{
   gl::AttachShader( mProgram, shader );
   mShaders.push_back( shader );
}

void ShaderProgram::Activate()
{
   Link();
   Validate();
   Use();
   CleanupShaders();
}

void ShaderProgram::Link()
{
   PerformAction( gl::LinkProgram, GL_LINK_STATUS, "linking" );
}

void ShaderProgram::Validate()
{
   PerformAction( gl::ValidateProgram, GL_VALIDATE_STATUS, "validating" );
}

void ShaderProgram::Use()
{
   gl::UseProgram( mProgram );
}

void ShaderProgram::CleanupShaders()
{
   for (auto shader : mShaders) { gl::DetachShader( mProgram, shader ); }
   for (auto shader : mShaders) { gl::DeleteShader( shader ); }
}

void ShaderProgram::PerformAction( std::function< void( gl::program_id ) > gl_func,
                                   GLenum status_type,
                                   const char* desc )
{
   GLint success = 0;
   GLchar error_log[1024];

   gl_func( mProgram );
   gl::GetProgramiv( mProgram, status_type, &success );
   if (!success) {
      gl::GetProgramInfoLog( mProgram, sizeof error_log, NULL, error_log );
      DBOUT( "Error " << desc << " shader program: '" << error_log << "'\n" );
      exit( 1 );
   }
}
