#include "utf8_utils.h"
#include <assert.h>

#ifdef _DEBUG

#define inline

#endif

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


inline int get_byte( int32_t n, size_t nb )
{
	assert( nb < sizeof( int32_t )  &&  "Invalid byte requested" );
	const int32_t bits_offset = nb * 8;
	const int32_t mask = 0xff << bits_offset;
	return (mask & n) >> bits_offset; 
}

inline int find_msb( int32_t utf8_char )
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

int utf8_trail( int32_t c )
{
	return bytes_for_utf8[ c & 0xFF ];
}

int32_t utf8_join_byte( int32_t current, int next )
{
	const int32_t mask1 = 0xC0;
	const int32_t mask2 = 0x80;
	assert( ( ( next & mask1 ) | mask2 ) == 0x80 && "Wrong unicode char" );	
	return ( current << 8 ) | next;
}

size_t utf8_len( int32_t utf8_char )
{
	return bytes_for_utf8[ 0xFF & utf8_char ] + 1;
}

int utf8_tostring( int32_t utf8_char, char* str )
{
	int msb = find_msb( utf8_char );
	int len = bytes_for_utf8[ 0xFF & msb ] + 1;

	if( len == 1 )
	{
		str[0] = (char) utf8_char;
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

int32_t utf8_fgetc( FILE* stream )
{
	int c = fgetc( stream );
	if( c != EOF )
	{
		int32_t utf8_char = c;
		int trail = utf8_trail( c );
		for( int j = 0; j != trail; ++j )
		{
			c = fgetc( stream );
			utf8_join_byte( utf8_char, c );
		}
		return utf8_char;
	}
	else
	{
		return c;
	}
}


int utf8_fread( int32_t *ptr_utf8_char, size_t count, FILE *stream )
{
	int chars_read = 0;
	for( int i = 0; i != count; ++i )
	{
		int c = fgetc( stream );
		if( c != EOF )
		{
			*( ptr_utf8_char + i ) = c;
			int trail = utf8_trail( c );
			for( int j = 0; j != trail; ++j )
			{
				c = fgetc( stream );
				*(ptr_utf8_char + i) = utf8_join_byte( *( ptr_utf8_char + i ), c );
			}
			chars_read++;
		}
		else
		{
			break;
		}	
	}
	return chars_read;
}

int32_t utf8_fputc( int32_t utf8_char, FILE* stream )
{
	int msb = find_msb( utf8_char );
	size_t char_size = utf8_len( msb );
	char* ptr_char = (char*)( &utf8_char );
	int32_t cwritten = fputc( ptr_char[ char_size - 1 ], stream );
	assert( cwritten == ( ptr_char[ char_size - 1 ] & 0xFF ) && "[utf8_fputc] failed to write utf8 char" );

	for( int i = 1; i != char_size; ++i )
	{
		int rc = fputc( ptr_char[ char_size - i - 1 ], stream );
		assert( rc == ( ptr_char[ char_size - i - 1 ] & 0xFF ) && "[utf8_fputc] failed to write utf8 char" );
		cwritten = utf8_join_byte( cwritten, rc );
	}
	return cwritten;	
}


int utf8_fwrite( int32_t *ptr_utf8_char, size_t count, FILE *stream )
{
	int chars_written = 0;
	for( int i = 0; i != count; ++i )
	{
		int32_t written = utf8_fputc( ptr_utf8_char[i], stream );
		assert( written == ptr_utf8_char[i] && "[utf8_fwrite] failed to write utf8 char" );
		chars_written++;
	}
	return chars_written;
}

