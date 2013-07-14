#pragma once

#include <GL3/gl3w.h>

//////////////////////////////
//
// Type safe wrapper for a class
// 
template <typename T, typename ID>
class IndexClass
{
public:
   IndexClass( T _val ):
      val( _val )
   {}

   T val;
};

#define DEFINE_INDEX_CLASS( _t, _name ) \
   class _name##_dummy;                 \
   using _name = IndexClass<_t, _name##_dummy>;

namespace gl
{
   // Type-safe wrappers for OpenGL's GLuints
   DEFINE_INDEX_CLASS( GLuint, buffer_id );
   DEFINE_INDEX_CLASS( GLuint, program_id );
   DEFINE_INDEX_CLASS( GLuint, location_id );
   DEFINE_INDEX_CLASS( GLuint, shader_id );
   DEFINE_INDEX_CLASS( GLuint, vertex_array_id );

   static_assert( sizeof( buffer_id[4] ) == sizeof( GLuint[4] ),
                  "buffer_id incorrect size" );

   const buffer_id null_buffer_id = buffer_id( 0 );

   // Type-safe rappers for OpenGL functions

   inline void       AttachShader( program_id program, shader_id shader )
   { return glAttachShader( program.val, shader.val ); }
   
   inline void       BindBuffer( GLenum target, buffer_id buffer )
   { return glBindBuffer( target, buffer.val ); }
   
   inline void       BindVertexArray( vertex_array_id array )
   { glBindVertexArray( array.val ); }
   
   inline void       CompileShader( shader_id shader )
   { glCompileShader( shader.val ); }

   inline program_id CreateProgram()
   { return program_id( glCreateProgram() ); }

   inline shader_id  CreateShader( GLenum shaderType )
   { return shader_id( glCreateShader( shaderType ) ); }

   inline void       GenBuffers( GLsizei n, buffer_id* buffers )
   { glGenBuffers( n, reinterpret_cast<GLuint*>( buffers ) ); }

   inline void       GenVertexArrays( GLsizei n, vertex_array_id* arrays )
   { glGenVertexArrays( n, reinterpret_cast<GLuint*>( arrays ) ); }

   inline void       GetProgramInfoLog( program_id program, GLsizei maxLength, GLsizei* length, GLchar* infoLog )
   { glGetProgramInfoLog( program.val, maxLength, length, infoLog ); }

   inline void       GetProgramiv( program_id program, GLenum pname, GLint* params )
   { glGetProgramiv( program.val, pname, params ); }

   inline void       GetShaderInfoLog( shader_id shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog )
   { glGetShaderInfoLog( shader.val, maxLength, length, infoLog ); }

   inline void       GetShaderiv( shader_id shader, GLenum pname, GLint* params)
   { glGetShaderiv( shader.val, pname, params ); }

   inline GLint      GetUniformLocation( program_id program, const GLchar* name )
   { return glGetUniformLocation( program.val, name ); }

   inline void       LinkProgram( program_id program )
   { return glLinkProgram( program.val ); }

   inline void       ShaderSource( shader_id shader, GLsizei count, const GLchar** string, const GLint* length )
   { glShaderSource( shader.val, count, string, length ); }

   inline void       Uniform3fv( location_id loc, GLsizei count, const GLfloat* value )
   { return glUniform3fv( loc.val, count, value ); }

   inline void       UniformMatrix3fv( location_id loc, GLsizei count, GLboolean transpose, const GLfloat* value )
   { return glUniformMatrix3fv( loc.val, count, transpose, value ); }

   inline void       UniformMatrix4fv( location_id loc, GLsizei count, GLboolean transpose, const GLfloat* value )
   { return glUniformMatrix4fv( loc.val, count, transpose, value ); }

   inline void       UseProgram( program_id program )
   { glUseProgram( program.val ); }

   inline void       ValidateProgram( program_id program )
   { return glValidateProgram( program.val ); }
}

//////////////////////////////
//
// RAII class for temporarily binding a vertex array object
//
class VertexArrayBinder
{
public:
   VertexArrayBinder( gl::vertex_array_id vao ) { gl::BindVertexArray( vao ); }
   ~VertexArrayBinder() { gl::BindVertexArray( 0 ); }
};

#define WithVertexArray( _vao ) VertexArrayBinder tmp_vab ( _vao );
