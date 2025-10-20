/*
 * main.c
 *
 * Copyright (C) 2025.10.20 TOP WAYE topwaye@hotmail.com
 * 
 * copy and replace pattern words with predefined words
 * 
 * a wildcard (*) matches zero or more characters
 */

#include <stdio.h>

#define COPY_AND_REPLACE	0 /* replace a matched string with a new string */
#define COPY_AND_APPEND		1 /* insert a new string after a matched string */
#define COPY_AND_INSERT		2 /* insert a new string before a matched string */

char pattern [ ] = "*H*****L***";
char replace [ ] = "hello world";

int copy_and_replace ( char * src, int src_len, char * dst, int dst_size, int how = COPY_AND_REPLACE )
{
	char * pos;
	int h, i, k;
	int a, b, bb, ii;

	if ( dst_size < 1 ) /* size >= len + 1 */
		return 0;

	h = 0, i = 0; 
	while ( i < src_len )
	{
		a = 0, b = 0;

		pos = pattern;

		k = 0; 
		while ( *( pos + k ) )
		{
			if ( a == 0 && b == 0 )
			{
				b = '*' == *( pos + k );

				if ( ! b )
				{
					if ( * ( src + i ) != *( pos + k ) )
						break;

					printf ( "%c", *( src + i ) );
					ii = i ++;
				}
				else
				{
					printf ( "%c", *( src + i ) );
					ii = i;
				}

				a = 1;
			}
			else if ( a == 1 && b == 1 )
			{
				bb = '*' == *( pos + k );

				if ( i == src_len )
				{
					printf ( "^\n" );
					a = 0;
					i = ii;
					break;
				}
	
				if ( ! bb )
				{
					if ( *( src + i ) != *( pos + k ) )
					{
						printf ( "%c", *( src + i ) );
						i ++;
						continue;
					}

					printf ( "%c", *( src + i ) );
					b = 0;
					i ++;
				}
				else
				{
					printf ( "%c", *( src + i ) );
				}
			}
			else if ( a == 1 && b == 0 )
			{
				b = '*' == *( pos + k ); /* must be here, do NOT move this line */

				if ( i == src_len )
				{
					printf ( "^\n" );
					a = 0;
					i = ii;
					break;
				}

				if ( ! b )
				{ 
					if ( *( src + i ) != *( pos + k ) )
					{
						printf ( "?\n" );
						a = 0;
						i = ii;
						break;
					}

					printf ( "%c", *( src + i ) );
					i ++;
				}
				else
				{
					printf ( "%c", *( src + i ) );
				}
			}
			else /* a == 0 && b == 1 */
			{
				/* never be here */
			}

			k ++;
		} /* end while */

		if ( a )
		{
			printf ( "<--OK\n" );

			if ( b ) /* must be here, do NOT move this line */
				i = src_len;

			if ( how == COPY_AND_APPEND )
			while ( ii < i )
			{
				if ( h + 1 == dst_size )
					return 0;

				*( dst + h ++ ) = *( src + ii ++ );
			}

			pos = replace;

			k = 0;
			while ( *( pos + k ) )
			{			
				if ( h + 1 == dst_size )
					return 0;

				*( dst + h ++ ) = *( pos + k ++ );
			}

			if ( how == COPY_AND_INSERT )
			while ( ii < i )
			{
				if ( h + 1 == dst_size )
					return 0;

				*( dst + h ++ ) = *( src + ii ++ );
			}

			continue;
		}
		
		if ( h + 1 == dst_size )
			return 0;

		*( dst + h ++ ) = *( src + i ++ );
	}

	*( dst + h ) = 0;

	return h;
}

#define MAX_TEST_SIZE	128

int main ( )
{
	char unknown_chars_1 [ ] = "x<HTML>x<HTM>xSELECTxHLx";
	char unknown_chars_2 [ MAX_TEST_SIZE ];

	int len_1 = sizeof ( unknown_chars_1 ) / sizeof ( unknown_chars_1 [ 0 ] ) - 1;
	int len_2 = 0;

	printf ( "--------------------------------------\n%d:%s\n", len_1, unknown_chars_1 );
	len_2 = copy_and_replace ( unknown_chars_1, len_1, unknown_chars_2, MAX_TEST_SIZE );
	printf ( "%d:%s\n", len_2, unknown_chars_2 );
	
	printf ( "--------------------------------------\n%d:%s\n", len_1, unknown_chars_1 );
	len_2 = copy_and_replace ( unknown_chars_1, len_1, unknown_chars_2, MAX_TEST_SIZE, COPY_AND_APPEND );
	printf ( "%d:%s\n", len_2, unknown_chars_2 );

	printf ( "--------------------------------------\n%d:%s\n", len_1, unknown_chars_1 );
	len_2 = copy_and_replace ( unknown_chars_1, len_1, unknown_chars_2, MAX_TEST_SIZE, COPY_AND_INSERT );
	printf ( "%d:%s\n", len_2, unknown_chars_2 );

	return 0;
}
