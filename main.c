/*
 * main.c
 *
 * Copyright (C) 2025.10.25 TOP WAYE topwaye@hotmail.com
 * 
 * copy and replace pattern words with predefined words
 * a wildcard (*) matches zero or more characters
 * 
 * as a result:
 * sign # represents a runtime current matched string which can NOT be trimmed
 * sign @ represents a runtime current matched string which can be trimmed (e.g. exclude somechar)
 * sign & represents a runtime placeholder which can be arbitrary string predefined
 */

#include <stdio.h>

int seek ( char c, char * src, int src_len, int * current )
{
	int i;
	int a;

	i = * current;
	
	a = 0;
	while ( i < src_len )
	{
		if ( c == *( src + i ) )
		{
			a = 1;
			break;	
		}	
	}

	* current = i;

	return a;
}

int do_match_ex ( char * target, char * src, int src_len, int * next )
{
	char * pos;
	int i, k;
	int a, b, bb, ii;

	i = * next;

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

				ii = i ++;
			}
			else
			{
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
					a = 0;
					i = ii;
					break;
				}

				if ( *( src + i ) != *( pos + k ) )
				{
					i ++;
					continue;
				}

				b = 0;
				i ++;
			}
		}
		else if ( a == 1 && b == 0 )
		{
			b = '*' == *( pos + k );

			if ( ! b )
			{
				if ( i == src_len ) /* must be here, do NOT move this line */
				{
					a = 0;
					i = ii;
					break;
				}

				if ( *( src + i ) != *( pos + k ) )
				{
					a = 0;
					i = ii;
					break;
				}

				i ++;
			}
		}
		else /* a == 0 && b == 1 */
		{
			/* never be here */
		}

		k ++;
	} /* end while */

	if ( a && b ) /* b must be after a, do NOT move this line */
		i = src_len;

	* next = i;

	return a;
}

int copy_and_replace_ex ( char * src, int src_len, char * dst, int dst_size,
						  char * pattern, char * replace, char * exclude,
						  ... )
{
	char * pos, * posx;
	int i, ii, j, h, k, s, t;
	va_list args;

	if ( dst_size < 1 ) /* size >= len + 1 */
		return 0;

	h = 0, i = 0; 
	while ( i < src_len )
	{
		ii = i; /* save i = current index */

		if ( ! do_match_ex ( pattern, src, src_len, & i ) )
		{
			if ( h + 1 == dst_size )
				return 0;

			*( dst + h ++ ) = *( src + i ++ );
			
			continue;
		}
	
		va_start ( args, exclude );

		pos = replace;

		k = 0;
		while ( *( pos + k ) )
		{
			if ( h + 1 == dst_size )
				return 0;

			if ( '@' == *( pos + k ) )
			{
				j = ii;
				while ( j < i )
				{
					if ( h + 1 == dst_size )
						return 0;

					posx = exclude;

					s = 0, t = 0;
					while ( *( posx + t ) )
					{
						if ( *( src + j ) == *( posx + t ) )
						{
							s = 1;
							break;
						}
						t ++;
					}
					if ( s )
					{
						j ++;
						continue;
					}

					*( dst + h ++ ) = *( src + j ++ );
				}

				k ++;
				continue;
			}

			if ( '#' == *( pos + k ) )
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

			if ( '&' == *( pos + k ) )
			{
				posx = va_arg ( args, char * );

				t = 0;
				while ( *( posx + t ) )
				{
					if ( h + 1 == dst_size )
						return 0;

					*( dst + h ++ ) = *( posx + t ++ );
				}

				k ++;
				continue;
			}

			*( dst + h ++ ) = *( pos + k ++ );
		}

		va_end ( args );
	}

	*( dst + h ) = 0;

	return h;
}

int do_match ( char * target, char * src, int src_len, int * next )
{
	char * pos;
	int i, k;
	int a, ii;

	i = * next;

	a = 0;
	pos = target;
	k = 0; 
	while ( *( pos + k ) )
	{
		if ( ! a )
		{
			if ( *( src + i ) != *( pos + k ) )
				break;

			printf ( "%c", *( src + i ) );
			a = 1;
			ii = i ++;
		}
		else
		{
			if ( i == src_len )
			{
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

		k++;
	} /* end while */

	* next = i;

	return a;
}

/* prototype match_ex function */
int match_ex ( char * pattern, char * src, int src_len, int granularity = 0 )
{
	int i, ii; 

	i = 0;
	while ( i < src_len )
	{
		ii = i; /* save i = current index */

		if ( ! do_match_ex ( pattern, src, src_len, & i ) )
		{
			/* match failed when i = current index, i is not changed */

			if ( granularity && ! seek ( '\n', src, src_len, & i ) ); /* start a new line hopefully */
				break;

			i ++; /* add 1 manually */

			continue;
		}

		/* match succeeded when i = current index, i is changed to next index automatically */

		return i - ii; /* matched string length */
	}

	return 0;
}

/* prototype match function */
int match ( char * target, char * src, int src_len, int granularity = 0 )
{
	int i, ii; 

	i = 0;
	while ( i < src_len )
	{
		ii = i; /* save i = current index */

		if ( ! do_match ( target, src, src_len, & i ) )
		{
			/* match failed when i = current index, i is not changed */

			if ( granularity && ! seek ( '\n', src, src_len, & i ) ); /* start a new line hopefully */
				break;

			i ++; /* add 1 manually */

			continue;
		}

		/* match succeeded when i = current index, i is changed to next index automatically */

		return i - ii; /* matched string length */
	}

	return 0;
}

int copy_and_replace ( char * src, int src_len, char * dst, int dst_size,
					   char * target, char * replace )
{
	int i, h, k;

	if ( dst_size < 1 ) /* size >= len + 1 */
		return 0;

	h = 0, i = 0; 
	while ( i < src_len )
	{
		if ( ! do_match ( target, src, src_len, & i ) )
		{
			if ( h + 1 == dst_size )
				return 0;

			*( dst + h ++ ) = *( src + i ++ );
			
			continue;
		}

		k = 0;
		while ( *( replace + k ) )
		{
			if ( h + 1 == dst_size )
				return 0;

			*( dst + h ++ ) = *( replace + k ++ );
		}
	}

	*( dst + h ) = 0;

	return h;
}

#define MAX_TEST_SIZE	128

int main ( )
{
	char target [ ] = "HTM";

	char pattern [ ] = "*H*****L***";
	char replace [ ] = "hello world";
	/*
	char replace [ ] = "#hello world";
	char replace [ ] = "@hello world";
	char replace [ ] = "&hello world";
	char replace [ ] = "&hello&world";
	*/
	char exclude [ ] = "\r\n"; /* what characters a matched @string excludes */

	char unknown_chars_1 [ ] = "x<HTML>x<HTM>xSELECTx";
	char unknown_chars_2 [ MAX_TEST_SIZE ];
	int len;

	len = sizeof ( unknown_chars_1 ) / sizeof ( unknown_chars_1 [ 0 ] ) - 1;

	printf ( "%d:%s\n", len, unknown_chars_1 );
	len = copy_and_replace ( unknown_chars_1, len, unknown_chars_2, MAX_TEST_SIZE, target, replace );
	printf ( "%d:%s\n", len, unknown_chars_2 );

	len = sizeof ( unknown_chars_1 ) / sizeof ( unknown_chars_1 [ 0 ] ) - 1;

	printf ( "%d:%s\n", len, unknown_chars_1 );
	len = copy_and_replace_ex ( unknown_chars_1, len, unknown_chars_2, MAX_TEST_SIZE, pattern, replace, exclude, "placeholder_1", "placeholder_2" );
	printf ( "%d:%s\n", len, unknown_chars_2 );

	return 0;
}
