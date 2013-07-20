#include <iostream>

#include "Math.h"
#include "MathTest.h"

void TestMatrixInverse()
{
   Vector3f rot( .6f, 1.2f, 1.8f );
   Matrix4f m;
   m.SetRotation( rot );
   Matrix4f n = m.OrthonormalInverse();

   std::cout << "Original matrix" << std::endl << m;
   std::cout << "Inverse" << std::endl << n;
   std::cout << "Product" << std::endl << n * m;
}
