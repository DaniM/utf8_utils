#include "utf8_utils.h"

using std::istream;
using std::ostream;
using std::ifstream;
using std::ostream;
using std::string;

// instantiate template methods
namespace utf8 = DaniM::Text::Utils;

/*
template class utf8::UTF8Char<uint32_t>;
template class utf8::UTF8Char<int32_t>;
template class utf8::UTF8Char<uint64_t>;
template class utf8::UTF8Char<int64_t>;

typedef utf8::UTF8Char<uint32_t> UChar32_t;
typedef utf8::UTF8Char<int32_t> Char32_t;
typedef utf8::UTF8Char<uint64_t> UChar64_t;
typedef utf8::UTF8Char<int64_t> Char64_t;


template
int utf8::get_byte<uint32_t>( uint32_t n, size_t nb );
template
int utf8::get_byte<int32_t>( int32_t n, size_t nb );
template
int utf8::get_byte<uint64_t>( uint64_t n, size_t nb );
template
int utf8::get_byte<int64_t>( int64_t n, size_t nb );

template 
int utf8::utf8_trail<uint32_t>( uint32_t c );
template 
int utf8::utf8_trail<int32_t>( int32_t c );
template 
int utf8::utf8_trail<uint64_t>( uint64_t c );
template 
int utf8::utf8_trail<int64_t>( int64_t c );

template
uint32_t utf8::utf8_join_byte<uint32_t>( uint32_t current, int next );

template
size_t utf8::utf8_len<uint32_t>( uint32_t utf8_char );

template
int utf8::find_msb<uint32_t>( uint32_t utf8_char );

template
string utf8::utf8_tostring<uint32_t>( uint32_t utf8_char );

template
int utf8::utf8_tostring<uint32_t>( uint32_t utf8_char, char* str );
*/

void utf8::open_utf8_file_reading( const char* filename, std::ifstream& in )
{
	in.open( filename, std::ifstream::binary );
}

void utf8::open_utf8_file_writing( const char* filename, std::ofstream& out )
{
	out.open( filename, std::ofstream::binary );
}

