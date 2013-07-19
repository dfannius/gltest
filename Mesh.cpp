#include "Mesh.h"
#include "TransformState.h"
#include "Utl.h"

Mesh::Mesh():
   mTransformState( new TransformState )
{
}

Mesh::~Mesh()
{
   delete mTransformState;
}

int Mesh::AddVertex( const Vector3f& pos, const Vector3f& color )
{
   mVertices.push_back( pos );

   Vector3f normal( pos );
   normal.Normalize();
   mVertices.push_back( normal );

   mVertices.push_back( color );

   return mVertices.size() / 3 - 1;
}

void Mesh::AddIndices( int i0, int i1, int i2 )
{
   mIndices.push_back( i0 );
   mIndices.push_back( i1 );
   mIndices.push_back( i2 );
}

void Mesh::RemoveTriangle( int tri_idx )
{
   mIndices.erase( mIndices.begin() + 3 * tri_idx,
                   mIndices.begin() + 3 * (tri_idx+1) );
}

void Mesh::Subdivide()
{
   const int num_tris = TriangleCount();
   for (int i = 0; i < num_tris; ++i) {
      SubdivideTriangle( 0 );
   }
}

int Mesh::AddAveragedVertex( int i, int j )
{
   const int i0 = Index( i );
   const int j0 = Index( j );

   const Vector3f& v = Vertex( i0 );
   const Vector3f& w = Vertex( j0 );

   Vector3f new_pos = (v + w) * 0.5f;
   new_pos.ScaleTo( v );
   Vector3f new_color = (Color( i0 ) + Color( j0 )) * 0.5f;
   
   const int new_idx = AddVertex( new_pos, new_color );
   return new_idx;
}

void Mesh::SubdivideTriangle( int tri_idx )
{
   const int idx_base = 3 * tri_idx;

   const int a = AddAveragedVertex( idx_base,     idx_base + 1 );
   const int b = AddAveragedVertex( idx_base + 1, idx_base + 2 );
   const int c = AddAveragedVertex( idx_base + 2, idx_base );

   const int i0 = Index( idx_base );
   const int i1 = Index( idx_base+1 );
   const int i2 = Index( idx_base+2 );

   AddIndices( i0, a, c );
   AddIndices( i1, b, a );
   AddIndices( i2, c, b );
   AddIndices( a, b, c );

   RemoveTriangle( tri_idx );
}

void Mesh::glVertexAttribPointer( int idx, DataType type )
{
   ::glVertexAttribPointer( idx,                                      // layout id
                            NumVertexDataTypes(),                     // # of components
                            GL_FLOAT,                                 // type of components
                            GL_FALSE,                                 // normalize if fixed-point
                            NumVertexDataTypes() * sizeof( Vector3f), // stride
                            (void *)( raw_enum( type ) * sizeof( Vector3f ) ) );  // offset
}

//////////////////////////////

Mesh* NewSphere()
{
   Mesh* mesh = new Mesh;

   Vector3f vertices[] = {      // Octahedron
      Vector3f(  1.f,  0.f,  0.f ),
      Vector3f( -1.f,  0.f,  0.f ),
      Vector3f(  0.f,  1.f,  0.f ),
      Vector3f(  0.f, -1.f,  0.f ),
      Vector3f(  0.f,  0.f,  1.f ),
      Vector3f(  0.f,  0.f,  -1.f ),
   };

   Vector3f colors[] = {
      Vector3f(  1.0f, 0.0f, 0.0f ),
      Vector3f(  0.7f, 0.7f, 0.0f ),
      Vector3f(  0.0f, 1.0f, 0.0f ),
      Vector3f(  0.0f, 0.7f, 0.7f ),
      Vector3f(  0.0f, 0.0f, 1.0f ),
      Vector3f(  0.7f, 0.0f, 0.7f ),
   };

   for (size_t i = 0; i < DIM( vertices ); ++i) {
      mesh->AddVertex( vertices[i], colors[i] );
   }

   int indices[][3] = {
      {0, 2, 4},
      {2, 1, 4},
      {1, 3, 4},
      {3, 0, 4},
      {2, 0, 5},
      {1, 2, 5},
      {3, 1, 5},
      {0, 3, 5}
   };

   for (size_t i = 0; i < DIM( indices ); ++i) {
      mesh->AddIndices( indices[i][0], indices[i][1], indices[i][2] );
   }

   for (int i = 0; i < 4; ++i) {
      mesh->Subdivide();
   }

   return mesh;
}

void Mesh::SetScale( const Vector3f& scale )
{
   mTransformState->SetScale( scale );
}

void Mesh::SetTranslation( const Vector3f& translation )
{
   mTransformState->SetTranslation( translation );
}

void Mesh::SetRotation( const Vector3f& rotation )
{
   mTransformState->SetRotation( rotation );
}

const Vector3f& Mesh::GetScale()
{
   return mTransformState->GetScale();
}

const Vector3f& Mesh::GetTranslation()
{
   return mTransformState->GetTranslation();
}

const Vector3f& Mesh::GetRotation()
{
   return mTransformState->GetRotation();
}
