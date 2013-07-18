#ifndef GLTEST_MATH_H
#define GLTEST_MATH_H

#include <cstring>
#include <ostream>
#include <math.h>

const double PI = 3.14159265;

struct Matrix4f;

//////////////////////////////

struct Vector3f
{
   Vector3f() {}
   Vector3f( float _x, float _y, float _z ):
      x( _x ), y( _y ), z( _z ) {}

   float* v() { return &x; }
   const float* v() const { return &x; }

   float& operator[]( int i ) { return v()[i]; };
   const float& operator[]( int i ) const { return v()[i]; };

   void SetZero() { x = y = z = 0.f; }
   void SetOne() { x = y = z = 1.f; }

   float Size() const { return sqrtf( x*x + y*y + z*z ); }

   void ScaleTo( float new_size )
   {
      const float scale = new_size / Size();
      *this *= scale;
   }

   void ScaleTo( const Vector3f& v )
   {
      const float scale = v.Size() / Size();
      *this *= scale;
   }

   void Normalize() { ScaleTo( 1.0f ); }

   Vector3f& operator += ( const Vector3f & rhs )
   {
      x += rhs.x; y += rhs.y; z += rhs.z;
      return *this;
   }

   Vector3f& operator -= ( const Vector3f & rhs )
   {
      x -= rhs.x; y -= rhs.y; z -= rhs.z;
      return *this;
   }

   Vector3f& operator *= ( float v )
   {
      x *= v; y *= v; z *= v;
      return *this;
   }

   Vector3f& operator /= ( float v )
   {
      x /= v; y /= v; z /= v;
      return *this;
   }

   float x, y, z;
};

inline Vector3f operator+( Vector3f a, const Vector3f& b )
{
   return a += b;
}

inline Vector3f operator-( Vector3f a, const Vector3f& b )
{
   return a -= b;
}

inline Vector3f operator*( Vector3f a, float b )
{
   return a *= b;
}

inline Vector3f operator/( Vector3f a, float b )
{
   return a /= b;
}

/*
inline void AddInto( Vector3f& res, const Vector3f& v, const Vector3f& w )
{
   res.x = v.x + w.x;
   res.y = v.y + w.y;
   res.z = v.z + w.z;
}

inline void SubtractInto( Vector3f& res, const Vector3f& v, const Vector3f& w )
{
   res.x = v.x - w.x;
   res.y = v.y - w.y;
   res.z = v.z - w.z;
}

inline void NegateInto( Vector3f& res, const Vector3f& v )
{
   res.x = -v.x;
   res.y = -v.y;
   res.z = -v.z;
}
*/

//////////////////////////////

struct Vector4f
{
   Vector4f() {}
   Vector4f( const Vector3f& v ):
      x( v.x ), y( v.y ), z( v.z ), w( 1.f ) {}
   Vector4f( float _x, float _y, float _z, float _w ):
      x( _x ), y( _y ), z( _z ), w( _w ) {}

   float* v() { return &x; }
   const float* v() const { return &x; }

   float& operator[]( int i ) { return v()[i]; };
   const float& operator[]( int i ) const { return v()[i]; };

   float x, y, z, w;
};

//////////////////////////////

enum class MatrixType
{
   kZeroMatrix,
   kIdentityMatrix,
   kMultiplicationMatrix
};

//////////////////////////////
//
// Matrices are row-major

struct Matrix3f
{
   Matrix3f()
   {
      memset( m, 0, 9 * sizeof( float ) );
   }
   Matrix3f( const Matrix4f& rhs );

   float& operator() ( int x, int y ) { return m[x][y]; }
   const float& operator() ( int x, int y ) const { return m[x][y]; }

   Matrix3f& operator=( const Matrix3f& rhs )
   {
      memcpy( m, rhs.m, 9 * sizeof( float ) );
      return *this;
   }

   Matrix3f( MatrixType type )
   {
      switch (type)
      {
         case MatrixType::kZeroMatrix:
         {
            memset( m, 0, 9 * sizeof( float ) );
            break;
         }
         case MatrixType::kIdentityMatrix:
         {
            memset( m, 0, 9 * sizeof( float ) );
            m[0][0] = m[1][1] = m[2][2] = 1.f;
            break;
         }
         default:
            break;
      }
   }

   Matrix3f( MatrixType type, const Matrix3f& m1, const Matrix3f& m2 )
   {
      switch (type)
      {
         case MatrixType::kMultiplicationMatrix:
         {
            for (unsigned int i = 0 ; i < 3 ; i++) {
               for (unsigned int j = 0 ; j < 3 ; j++) {
                  m[i][j] = (m1.m[i][0] * m2.m[0][j] +
                             m1.m[i][1] * m2.m[1][j] +
                             m1.m[i][2] * m2.m[2][j]);
               }
            }
         }
         default:
            break;
      }
   }

   void SetZero()
   {
      memset( m, 0, 9 * sizeof( float ) );
   }

   void SetIdentity()
   {
      memset( m, 0, 9 * sizeof( float ) );
      m[0][0] = m[1][1] = m[2][2] = 1.f;
   }
   float const* Data() const { return &m[0][0]; }

   float m[3][3];
};

//////////////////////////////

struct Matrix4f
{
   Matrix4f()
   {
      memset( m, 0, 16 * sizeof( float ) );
   }
      
   float& operator() ( int x, int y ) { return m[x][y]; }
   const float& operator() ( int x, int y ) const { return m[x][y]; }

   Matrix4f& operator=( const Matrix4f& rhs )
   {
      memcpy( m, rhs.m, 16 * sizeof( float ) );
      return *this;
   }

   Matrix4f( MatrixType type )
   {
      switch (type)
      {
         case MatrixType::kZeroMatrix:
         {
            memset( m, 0, 16 * sizeof( float ) );
            break;
         }
         case MatrixType::kIdentityMatrix:
         {
            memset( m, 0, 16 * sizeof( float ) );
            m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
            break;
         }
         default:
            break;
      }
   }

   Matrix4f( MatrixType type, const Matrix4f& m1, const Matrix4f& m2 )
   {
      switch (type)
      {
         case MatrixType::kMultiplicationMatrix:
         {
            for (unsigned int i = 0 ; i < 4 ; i++) {
               for (unsigned int j = 0 ; j < 4 ; j++) {
                  m[i][j] = (m1.m[i][0] * m2.m[0][j] +
                             m1.m[i][1] * m2.m[1][j] +
                             m1.m[i][2] * m2.m[2][j] +
                             m1.m[i][3] * m2.m[3][j]);
               }
            }
         }
         default:
            break;
      }
   }

   void SetZero()
   {
      memset( m, 0, 16 * sizeof( float ) );
   }

   void SetIdentity()
   {
      memset( m, 0, 16 * sizeof( float ) );
      m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
   }

   void SetScale( const Vector3f& v );
   void SetRotation( const Vector3f& v ); // v's angles are in radians
   void SetTranslation( const Vector3f& v );

   const float* Data() const { return &m[0][0]; }

   float m[4][4];
};

//////////////////////////////

class TransformState
{
public:
   TransformState();

   void SetScale( const Vector3f& scale ) { mScale = scale; }
   void SetTranslation( const Vector3f& translation ) { mTranslation = translation; }
   void SetRotation( const Vector3f& rotation ) { mRotation = rotation; }

   const Vector3f& GetScale() { return mScale; }
   const Vector3f& GetTranslation() { return mTranslation; }
   const Vector3f& GetRotation() { return mRotation; }

public:
   Vector3f mScale;
   Vector3f mTranslation;
   Vector3f mRotation;
};

//////////////////////////////

std::ostream& operator<< (std::ostream &out, const Matrix3f &m);
std::ostream& operator<< (std::ostream &out, const Matrix4f &m);
std::ostream& operator<< (std::ostream &out, const Vector3f &v);
std::ostream& operator<< (std::ostream &out, const Vector4f &v);

/*
void MultiplyInto( Matrix4f& m, const Matrix4f& m1, const Matrix4f& m2 );
void MultiplyInto( Vector4f& vout, const Matrix4f& m, const Vector4f& v );
*/

Matrix4f operator*( const Matrix4f& m1, const Matrix4f& m2 );
Vector4f operator*( const Matrix4f& m, const Vector4f& v );

float degrees_to_radians( float x );
float radians_to_degrees( float x );

#endif
