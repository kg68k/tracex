/*

  decode arguments

  Copyright (C) 1991 K.Abe

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <jctype.h>
#include "trace.h"
#include "syscall.h"

static void	decode_argument( int , void* , char* , System_call* , char* , int );
static void	decode_argument_by_letter( char* , char* , void* );
static void	disk( int , void* , char* );


static System_call	System_call_info[256];


extern void	Initialize_argument_information()
{
    int	i;

    for( i = 0 ; i < 256 ; i++ ) {
	System_call_info[i].name = NULL;
    }

    for( i = 0 ; i < sizeof( Human_system_call ) / sizeof( Human_system_call[0] ) ; i++ ) {
	System_call_info[ Human_system_call[i].number ].name =
	    Human_system_call[i].name;
	System_call_info[ Human_system_call[i].number ].argletter =
	    Human_system_call[i].argletter;
    }
}


#define DECODE(first,info_name)\
    decode_argument( *(first*)arg & 0xff, arg , argbuf ,\
		     info_name##_call_info ,\
		     #info_name ,\
		     sizeof(info_name##_call_info)/sizeof(info_name##_call_info[0]))

extern char	*Format_output( doscall , arg )
int	doscall;
void	*arg;
{
    static char argbuf[256];

    if( doscall < 0 || doscall > 256 || System_call_info[ doscall ].name == NULL ) {
	sprintf( argbuf , "dos(0x%x){UNDEFINED}" , doscall );
	return argbuf;
    }
    switch( doscall ) {
    case 0x0c:	/* kflush */
	DECODE( short , Kflush );
	break;
    case 0x18:	/* hendsp */
	DECODE( short , Hendsp );
	break;
    case 0x44:	/* ioctrl */
	DECODE( short , Ioctrl );
	break;
    case 0x22:	/* knjctrl */
	DECODE( long , Knjctrl );
	break;
    case 0x23:	/* conctrl */
	DECODE( short , Conctrl );
	break;
    case 0x24:	/* keyctrl */
	DECODE( short , Keyctrl );
	break;
    case 0x4b:	/* exec */
	DECODE( short , Exec );
	break;
    case 0x55:	/* common */
	DECODE( short , Common );
	break;
    case 0x5f:	/* getassign */
	DECODE( short , Getassign );
	break;
    case 0xf3:	/* diskred */
    case 0xf4:	/* diskwrt */
	disk( doscall , arg , argbuf );
	break;
    default:
	decode_argument( doscall , arg , argbuf  , System_call_info , "dos" , 256 );
	break;
    }
    return argbuf;
}


static void	escape( str , buffer )
char	*str , *buffer;
{
    int	count;

    if( str >= (char*)0x01000000 ) {
	sprintf( buffer , "(0x%x)" , str );
	buffer += strlen( buffer );
	str = (char*)( (unsigned long)str & 0xffffff );
    }

    *buffer++ = '\"';
    for( count = 0 ; *str && count < 32 ; str++ , count++ ) {
	if( isprkana( *str ) )
	    *buffer++ = *str;
	else if( iscntrl( *str ) )
	    sprintf( buffer , "\\x%x" , *str );
	else if( iskanji( *str ) && iskanji2( *( str + 1 ) ) ) {
	    *buffer++ = *str++;
	    *buffer++ = *str;
	    count++;
	} else
	    *buffer++ = '.';
    }
    *buffer++ = '\"';
    if( *str ) {
	*buffer++ = '.';
	*buffer++ = '.';
    }
    *buffer++ = '\0';

}


static void	decode_argument( call , arg , buffer , info , name , max )
int		call;
void		*arg;
char		*buffer;
System_call	*info;
char		*name;
int		max;
{
    if( call >= max || info[ call ].name[0] == '\0' ) {
	sprintf( buffer , "%s(%d){UNDEFINED}" , name , call );
	return;
    }
    strcpy( buffer ,  info[ call ].name );
    decode_argument_by_letter( buffer + strlen( buffer ) , info[ call ].argletter , arg );
}


static void	decode_argument_by_letter( buffer , argletter , arg )
char	*buffer;
char	*argletter;
void	*arg;
{
    char temp[256];
    *buffer++ = '(';

    while( *argletter ) {
	switch( *argletter ) {
	case 'w':
	    sprintf( buffer , "%d" , *(short*)arg );
	    if( *(unsigned short*)arg >= 10 ) {
		buffer += strlen( buffer );
		sprintf( buffer , "(0x%x)" , *(short*)arg & 0xffff );
	    }
	    arg += 2;
	    break;
	case 'l':
	    sprintf( buffer , "%d" , *(long*)arg );
	    if( *(long*)arg >= 10 ) {
		buffer += strlen( buffer );
		sprintf( buffer , "(0x%x)" , *(long*)arg );
	    }
	    arg += 4;
	    break;
	case 'p':
	    sprintf( buffer , "0x%x" , *(long*)arg );
	    arg += 4;
	    break;
	case 's':
	    escape( *(long*)arg , temp );
	    sprintf( buffer , "%s" , temp );
	    arg += 4;
	    break;
	default:
	    goto exit;
	}
	buffer += strlen( buffer );
	if( *++argletter )
	    *buffer++ = ',';
    }
 exit:
    *buffer++ = ')';
    *buffer = '\0';
}


static void	disk( doscall , arg , argbuf )
int	doscall;
void	*arg;
char	*argbuf;
{
    char	*argletter;

    strcpy( argbuf ,  System_call_info[ doscall ].name );
    argbuf += strlen( argbuf );
    if( *(char*)arg & 0x80 ) {
	strcpy( argbuf , "{2}" );
	argbuf += strlen( argbuf );
	argletter = "pwll";
    } else
	argletter = "pwww";

    decode_argument_by_letter( argbuf , argletter , arg );
}
