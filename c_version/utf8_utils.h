#include <stdint.h>
#include <stdio.h>

int utf8_trail( int32_t c );

int32_t utf8_join_byte( int32_t current, int next );

size_t utf8_len( int32_t utf8_char );

int utf8_tostring( int32_t utf8_char, char* str );


////////////////////////
// reading functions
///////////////////////
int32_t utf8_fgetc( FILE* stream );

int utf8_fread( int32_t *ptr_utf8_char, size_t count, FILE *stream );


//////////////////////
// writing functions
/////////////////////
int utf8_fputc( int32_t utf8_char, FILE* stream );

int utf8_fwrite( int32_t *ptr_utf8_char, size_t count, FILE *stream );

