#include <cstdlib>
#include <fstream>
#include <iterator>

#include "Utl.h"

void ReadFileToString( const char* filename, std::string& buf )
{
   std::ifstream in( filename );
   in >> std::noskipws;
   copy( std::istreambuf_iterator<char>( in ),
         std::istreambuf_iterator<char>(),
         inserter( buf, buf.begin() ) );
}

float RndFloat()
{
   return float( rand() ) / RAND_MAX;
}
