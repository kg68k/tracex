/*

  display doscall name, arguments, result code
  ( called from New_handler )

  Copyright (C) 1991 K.Abe

*/

#include <stdio.h>
#include <iocslib.h>
#include "trace.h"
#include "error.inc"

extern void	display2( int , int );
static void	indent( void );

static char	*Formatted_string;
static int	Flush_flag = 1;
static int	Doscall_nestlevel = 0;
static int	Last_instruction_not_return = 0;


extern void	display1( callnum , arg )
int	callnum;
void	*arg;
{
    callnum &= 0xff;

    if( Flush_flag == 0 ) {
	display2( 0 , 1 );
	Doscall_nestlevel++;
    }
    Formatted_string = Format_output( callnum , arg );
    Flush_flag = 0;
    Count++;
    if( callnum == 0 || callnum == 0x4c || callnum == 0x31 ) {
	Last_instruction_not_return = 1;
	Doscall_nestlevel--;
    } else
	Last_instruction_not_return = 0;
}


extern void	display2( result , result_unknown )
int	result;
int	result_unknown;
{
    if( isatty( fileno( Stream ) ) && ( B_LOCATE( -1 , -1 ) & 0xffff0000 ) != 0 )
	putc( '\n' , Stream );

    if( Flush_flag )
	Doscall_nestlevel--;

    indent();
    if( Flush_flag )
	putc( '}' , Stream );
    else
	fputs( Formatted_string , Stream );

    putc( '=' , Stream );
    if( result_unknown ) {
	if( Last_instruction_not_return )
	    putc( '?' , Stream );
	else
	    putc( '{' , Stream );
    } else {
	fprintf( Stream , "%d" , result );
	if( (unsigned long)result >= 10 )
	    fprintf( Stream , "(0x%x)" , result );
	if( ERROR_NUMBER_MIN <= result && result < 0 )
	    fprintf( Stream , "(%s)" , Human_error_message[ -result ] );
    }

    putc( '\n' , Stream );
    Flush_flag = 1;
}


static void	indent()
{
    int	i;
    for( i = 0 ; i < Doscall_nestlevel ; i++ ) {
	putc( ' ' , Stream );
	putc( ' ' , Stream );
    }
}
