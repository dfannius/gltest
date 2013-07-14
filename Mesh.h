#pragma once

#include <vector>

#include "Math.h"
#include "Utl.h"

class TransformState;

class Mesh
{
public:
   Mesh();
   ~Mesh();

   enum class DataType {
      kVertexPos,
      kVertexNormal,
      kVertexColor,
      kNumVertexDataTypes
   };

   int NumVertexDataTypes() const { return raw_enum( DataType::kNumVertexDataTypes ); }

   Vector3f& Data( int idx, DataType type ) {
      size_t raw_idx = NumVertexDataTypes() * idx + raw_enum( type );
      return mVertices[raw_idx];
   }

   Vector3f& Vertex( int idx ) { return Data( idx, DataType::kVertexPos ); }
   Vector3f& Normal( int idx ) { return Data( idx, DataType::kVertexNormal ); }
   Vector3f& Color( int idx )  { return Data( idx, DataType::kVertexColor ); }

   // Returns the index of the vertex just added
   int AddVertex( const Vector3f& pos, const Vector3f& color );
   const Vector3f* VertexData() const { return &mVertices[0]; }
   int VertexDataSize() const { return mVertices.size() * sizeof( Vector3f ); }
   int VertexDataCount() const { return mVertices.size(); }

   int Index( int idx ) const { return mIndices[idx]; }
   void AddIndices( int i0, int i1, int i2 );
   const int* IndexData() const { return &mIndices[0]; }
   int IndexDataSize() const { return mIndices.size() * sizeof( int ); }
   int IndexDataCount() const { return mIndices.size(); }
   int TriangleCount() const { return IndexDataCount() / 3; }

   void RemoveTriangle( int tri_idx );

   void Subdivide();

   void glVertexAttribPointer( int idx, DataType type );

   TransformState& GetTransformState() { return *mTransformState; }
   void SetScale( const Vector3f& scale );
   void SetTranslation( const Vector3f& translation );
   void SetRotation( const Vector3f& rotation );
   const Vector3f& GetScale();
   const Vector3f& GetTranslation();
   const Vector3f& GetRotation();

private:
   int AddAveragedVertex( int i, int j );
   void SubdivideTriangle( int tri_idx );

   std::vector<Vector3f> mVertices; // interleaved vertex/normal/color
   std::vector<int> mIndices;       // vertex indices of successive triangles
   TransformState* mTransformState;
};

Mesh* NewSphere();
