#include <iomanip>

#include "Math.h"

std::ostream& operator<< (std::ostream &out, Matrix3f const &m)
{
   for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
         out.precision( 2 );
         out << std::setw( 6 );
         out << m.m[i][j] << " ";
      }
      out << "\n";
   }
   return out;
}

std::ostream& operator<< (std::ostream &out, Matrix4f const &m)
{
   for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
         out.precision( 2 );
         out << std::setw( 6 );
         out << m.m[i][j] << " ";
      }
      out << "\n";
   }
   return out;
}

std::ostream& operator<< (std::ostream &out, Vector3f const &v)
{
   for (int i = 0; i < 3; ++i) {
      out.precision( 2 );
      out << std::setw( 6 );
      out << v[i] << " ";
   }
   return out;
}

std::ostream& operator<< (std::ostream &out, Vector4f const &v)
{
   for (int i = 0; i < 4; ++i) {
      out.precision( 2 );
      out << std::setw( 6 );
      out << v[i] << " ";
   }
   return out;
}

Matrix3f::Matrix3f( const Matrix4f& rhs )
{
   for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
         m[i][j] = rhs.m[i][j];
      }
   }
}


void MultiplyInto( Matrix4f& m, Matrix4f const& m1, Matrix4f const& m2 )
{
   for (unsigned int i = 0; i < 4; ++i) {
      for (unsigned int j = 0; j < 4; ++j) {
         m.m[i][j] = (m1.m[i][0] * m2.m[0][j] +
                      m1.m[i][1] * m2.m[1][j] +
                      m1.m[i][2] * m2.m[2][j] +
                      m1.m[i][3] * m2.m[3][j]);
      }
   }
}

/*
void MultiplyInto( Vector4f& vout, Matrix4f const& m, Vector4f const& v )
{
   for (int i = 0; i < 4; ++i) {
      vout[i] = (m.m[i][0] * v[0] +
                 m.m[i][1] * v[1] +
                 m.m[i][2] * v[2] +
                 m.m[i][3] * v[3]);
   }
}
*/

Matrix4f operator*( const Matrix4f& m1, const Matrix4f& m2 )
{
   Matrix4f m( MatrixType::kMultiplicationMatrix, m1, m2 );
   return m;
}

Vector4f operator*( const Matrix4f& m, const Vector4f& v )
{
   Vector4f vout;
   for (int i = 0; i < 4; ++i) {
      vout[i] = (m.m[i][0] * v[0] +
                 m.m[i][1] * v[1] +
                 m.m[i][2] * v[2] +
                 m.m[i][3] * v[3]);

   }
   return vout;
}

void Matrix4f::SetScale( const Vector3f& v )
{
   SetZero();
   m[0][0] = v.x;
   m[1][1] = v.y;
   m[2][2] = v.z;
   m[3][3] = 1.f;
}

void Matrix4f::SetRotation( const Vector3f& v )
{
   Matrix4f rx, ry, rz;

   float cosfx = cosf( v.x ); float sinfx = sinf( v.x );
   float cosfy = cosf( v.y ); float sinfy = sinf( v.y );
   float cosfz = cosf( v.z ); float sinfz = sinf( v.z );

   rx.SetIdentity();
   rx( 1, 1 ) = cosfx; rx( 1, 2 ) = -sinfx;
   rx( 2, 1 ) = sinfx; rx( 2, 2 ) = cosfx;

   ry.SetIdentity();
   ry( 0, 0 ) = cosfy; ry( 0, 2 ) = -sinfy;
   ry( 2, 0 ) = sinfy; ry( 2, 2 ) = cosfy;

   rz.SetIdentity();
   rz( 0, 0 ) = cosfz; rz( 0, 1 ) = -sinfz;
   rz( 1, 0 ) = sinfz; rz( 1, 1 ) = cosfz;

   MultiplyInto( *this, rx * ry, rz );
}

void Matrix4f::SetTranslation( const Vector3f& v )
{
   SetIdentity();
   m[0][3] = v.x;
   m[1][3] = v.y;
   m[2][3] = v.z;
}

float degrees_to_radians( float x )
{
   return float( x * PI / 180.f );
}

float radians_to_degrees( float x )
{
   return float( x * 180.f / PI );
}

TransformState::TransformState()
{
   mScale.SetOne();
   mTranslation.SetZero();
   mRotation.SetZero();
}

