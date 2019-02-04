#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

namespace DaniM
{

namespace Text
{

namespace Utils
{

static const char bytes_for_utf8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

template<typename T>
class UTF8Char
{
public:
	UTF8Char( ) : utf8_char( 0 ) {}
	explicit UTF8Char( T c ) : utf8_char( c ) {}
	explicit UTF8Char( char c ) : utf8_char( c & 0xFF ) 
	{
		assert( c < 0xF7 && "Invalid unicode char" );
	}
	T utf8_char;
};

typedef UTF8Char<uint32_t> UChar32_t;
typedef UTF8Char<int32_t> Char32_t;
typedef UTF8Char<uint64_t> UChar64_t;
typedef UTF8Char<int64_t> Char64_t;

typedef std::basic_string<Char32_t> U32_String;
typedef std::basic_string<Char64_t> U64_String;

template<typename T>
int get_byte( T n, size_t nb )
{
	assert( nb < sizeof( T )  &&  "Invalid byte requested" );
	const T bits_offset = nb * 8;
	const T mask = 0xff << bits_offset;
	return (mask & n) >> bits_offset; 
}

template <typename T>
int utf8_trail( T c )
{
	return bytes_for_utf8[ c & 0xFF ];
}

template<typename T>
T utf8_join_byte( T current, int next )
{
	const T mask1 = 0xC0;
	const T mask2 = 0x80;
	assert( ( ( next & mask1 ) | mask2 ) == 0x80 && "Wrong unicode char" );	
	return ( current << 8 ) | next;
}

template <typename T>
size_t utf8_len( T utf8_char )
{
	return bytes_for_utf8[ 0xFF & utf8_char ] + 1;
}

template<typename T>
int find_msb( T utf8_char )
{
	if( utf8_char <= 0x7F )
	{
		return utf8_char;
	}
	else
	{
		int byte = ( utf8_char >> 8 );
		while( ( ( byte & 0xFF ) >> 6 ) == 0x02 )
		{
			byte = byte >> 8;
		}
		return byte;
	}
}

template <typename T>
std::string utf8_tostring( T utf8_char )
{
	int msb = find_msb( utf8_char );
	int len = bytes_for_utf8[ 0xFF & msb ] + 1;

	if( len == 1 )
	{
		return std::string( 1, static_cast<char>( utf8_char ) );
	}
	else
	{
		std::string str( len, '\0' );

		for( int i = 0; i != len; ++i )
		{
			str[i] = get_byte( utf8_char, len - i - 1 );
		}
		return str;
	}
}

template <typename T>
int utf8_tostring( T utf8_char, char* str )
{
	int msb = find_msb( utf8_char );
	int len = bytes_for_utf8[ 0xFF & msb ] + 1;

	if( len == 1 )
	{
		str[0] = static_cast<char>( utf8_char );
		str[1] = '\0';
	}
	else
	{
		for( int i = 0; i != len; ++i )
		{
			str[ i ] = get_byte( utf8_char, len - i - 1 );
		}


		str[len] = '\0';
	}

	return len;
}

////////////////////////
// reading functions
///////////////////////

template<typename T>
std::istream& operator>>( std::istream& in, UTF8Char<T>& c )
{
	c.utf8_char = 0;
	c.utf8_char = in.get();
	if( in.good() )
	{
		int bytes = utf8_trail( c.utf8_char );
		for( int i = 0; i != bytes; ++i )
		{
			int next = in.get();
			c.utf8_char = utf8_join_byte( c.utf8_char, next );
		}
	}
	return in;
}

void open_utf8_file_reading( const char* filename, std::ifstream& in );


//////////////////////
// writing functions
/////////////////////


template<typename T>
std::ostream& operator <<( std::ostream& out, const UTF8Char<T>& c )
{
	int msb = find_msb( c.utf8_char );
	int len = bytes_for_utf8[ 0xFF & msb ] + 1;

	if( len == 1 )
	{
		out << static_cast<char>( c.utf8_char );
	}
	else
	{
		for( int i = 0; i != len; ++i )
		{
			out << static_cast<char>( get_byte( c.utf8_char, len - i - 1 ) );
		}
	}

	return out;
}

void open_utf8_file_writing( const char* filename, std::ofstream& out );

}
}
}
