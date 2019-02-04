#include <iostream>
#include <istream>
#include "utf8_utils.h"

using namespace std;

using namespace DaniM::Text::Utils;

int read_write_test_1()
{
	int val = 0x00E282AC;
	ifstream in( "utf8_test.in", std::ifstream::binary );
	ofstream out( "utf8_test1.out", std::ofstream::binary );
	int c = in.get();
	int bytes = utf8_trail( c );
	assert( bytes == 2 && "Invalid size for unicode char U+20AC" );
	for( int i = 0; i != bytes; ++i )
	{
		int next = in.get();
		c = utf8_join_byte( c, next );
	}
	out << utf8_tostring( c );
	assert( c == val && "Wrong unicode value" );

	c = in.get();
	while( in.good() )
	{
		int bytes = utf8_trail( c );
		for( int i = 0; i != bytes; ++i )
		{
			int next = in.get();
			c = utf8_join_byte( c, next );
		}
		out << utf8_tostring( c );
		c = in.get();
	}
	in.close();
	out.close();
	
	return 0;
}

int read_write_test_2()
{
	int val = 0x00E282AC;
	ifstream in( "utf8_test.in", std::ifstream::binary );
	ofstream out( "utf8_test2.out", std::ofstream::binary );

	Char32_t c;
	in >> c;

	out <<  c;
	assert( c.utf8_char == val && "Wrong unicode value" );

	while( in >> c )
	{
		out << c;
	}
	in.close();
	out.close();
	
	return 0;
}

int compare_files( const char* filename1, const char* filename2 )
{
	ifstream in1( filename1, std::ifstream::binary );
	ifstream in2( filename2, std::ifstream::binary );

	size_t chars_read = 0;

	Char32_t c1;
	Char32_t c2;
	in1 >> c1;
	in2 >> c2;
	while( in1.good() && in2.good() )
	{
		//cout << "Char: " << ++chars_read << "\n";
		assert( c1.utf8_char == c2.utf8_char && "not the same content" );
		in1 >> c1;
		in2 >> c2;
	}

	assert( in1.good() == in2.good() && "Not the same file state" );

	in1.close();
	in2.close();
}

int main( int argc, const char* argv[] )
{
	cout << "read/write test 1\n";
	read_write_test_1();
	compare_files( "utf8_test.in", "utf8_test1.out" );

	cout << "read/write test 2\n";
	read_write_test_2();
	compare_files( "utf8_test.in", "utf8_test2.out" );

	return 0;
}
