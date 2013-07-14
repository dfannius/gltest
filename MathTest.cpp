#include "Math.h"

float test( const Vector3f& a, const Vector3f& b )
{
   Vector3f c;
   AddInto( c, a, b );
   
   Vector3f d = a + b;

   return c.x + d.x;
}
