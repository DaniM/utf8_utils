#include "utf8_utils.h"
#include <assert.h>

int read_write_test_1()
{
	int val = 0x00E282AC;
	FILE* in = fopen( "utf8_test.in", "r" );
	FILE* out = fopen( "utf8_test1.out", "w" );
	int c = fgetc( in );
	int bytes = utf8_trail( c );
	assert( bytes == 2 && "Invalid size for unicode char U+20AC" );
	for( int i = 0; i != bytes; ++i )
	{
		int next = fgetc( in );
		c = utf8_join_byte( c, next );
	}
	assert( c == val && "Wrong unicode value" );

	char str[5];
	int size = utf8_tostring( c, str );
	fwrite( str, 1, size, out );

	c = fgetc( in );
	while( !feof( in ) )
	{
		int bytes = utf8_trail( c );
		for( int i = 0; i != bytes; ++i )
		{
			int next = fgetc( in );
			c = utf8_join_byte( c, next );
		}
		int size = utf8_tostring( c, str );
		fwrite( str, 1, size, out );

		c = fgetc( in );
	}

	fclose( in );
	fclose( out );
	
	return 0;
}

int read_write_test_2()
{
	int val = 0x00E282AC;
	FILE* in = fopen( "utf8_test.in", "r" );
	FILE* out = fopen( "utf8_test2.out", "w" );

	int32_t c;
	utf8_fread( &c, 1, in );
	assert( c == val && "Wrong unicode value" );

	utf8_fwrite( &c, 1, out );

	while( utf8_fread( &c, 1, in ) > 0 )
	{
		utf8_fwrite( &c, 1, out );
	}
	
	fclose( in );
	fclose( out );
	
	return 0;
}

int compare_files( const char* filename1, const char* filename2 )
{
	FILE* in1 = fopen( filename1, "r" );
	FILE* in2 = fopen( filename2, "r" );

	size_t chars_read = 0;

	int32_t c1;
	int32_t c2;

	utf8_fread( &c1, 1, in1 );
	utf8_fread( &c2, 1, in2 );

	while( !feof( in1 ) && !feof( in2 ) )
	{
		assert( c1 == c2 && "not the same content" );
		utf8_fread( &c1, 1, in1 );
		utf8_fread( &c2, 1, in2 );
	}

	assert( feof( in1 ) == feof( in2 ) && "Not the same file state" );

	fclose( in1 );
	fclose( in2 );
}

int main( int argc, const char* argv[] )
{
	printf( "read/write test 1\n" );
	read_write_test_1();
	compare_files( "utf8_test.in", "utf8_test1.out" );

	printf( "read/write test 2\n" );
	read_write_test_2();
	compare_files( "utf8_test.in", "utf8_test2.out" );

	return 0;
}
