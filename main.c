/*
 * main.c
 *
 * Copyright (C) 2025.10.21 TOP WAYE topwaye@hotmail.com
 * 
 * copy and replace pattern words with predefined words
 * 
 * a wildcard (*) matches zero or more characters
 * 
 * as a result, a number sign (#) represents a runtime current matched string
 */

#include <stdio.h>

char pattern [ ] = "*H*****L***";
char replace [ ] = "hello world";
/*
char replace [ ] = "#hello world";
char replace [ ] = "hello world#";
char replace [ ] = "#hello#world";
char replace [ ] = "hello##world";
*/

int copy_and_replace ( char * src, int src_len, char * dst, int dst_size )
{
	char * pos;
	int i, j, h, k;
	int a, b, bb, ii;
	int r;

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

				if ( ! bb )
				{
					if ( i == src_len ) /* must be here, do NOT move this line */
					{
						printf ( "^\n" );
						a = 0;
						i = ii;
						break;
					}

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
				b = '*' == *( pos + k );

				if ( ! b )
				{
					if ( i == src_len ) /* must be here, do NOT move this line */
					{
						printf ( "^\n" );
						a = 0;
						i = ii;
						break;
					}

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

			pos = replace;

			k = 0;
			while ( *( pos + k ) )
			{
				if ( h + 1 == dst_size )
					return 0;

				r = '#' == *( pos + k );
				
				if ( r )
				{
					j = ii;
					while ( j < i )
					{
						if ( h + 1 == dst_size )
							return 0;

						*( dst + h ++ ) = *( src + j ++ );
					}

					k ++;
					continue;
				}

				*( dst + h ++ ) = *( pos + k ++ );
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

	int len = sizeof ( unknown_chars_1 ) / sizeof ( unknown_chars_1 [ 0 ] ) - 1;

	printf ( "%d:%s\n", len, unknown_chars_1 );
	len = copy_and_replace ( unknown_chars_1, len, unknown_chars_2, MAX_TEST_SIZE );
	printf ( "%d:%s\n", len, unknown_chars_2 );

	return 0;
}
