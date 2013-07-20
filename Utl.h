#ifndef GLTEST_UTL_H
#define GLTEST_UTL_H

#include <GL3/gl3w.h>

#include <iosfwd>
#include <string>
#include <type_traits>

#if 0 
# define DBOUT( s )                           \
{                                            \
   std::ostringstream os_;                   \
   os_ << s;                                 \
   OutputDebugStringA( os_.str().c_str() );  \
}
#else
# define DBOUT( s ) std::cout << s
#endif

#define DIM( arr ) (sizeof arr / sizeof arr[0])

void ReadFileToString( const char* filename, std::string& buf );

float RndFloat();

template <typename E>
typename std::underlying_type<E>::type raw_enum( E e ) {
    return static_cast<typename std::underlying_type<E>::type>( e );
}

#endif
