/*

  System call tracer

  Copyright (C) 1991 K.Abe

*/

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <doslib.h>

#include "trace.h"

#define	FLINE_TRAP_VECTOR	11

typedef void (*handler)(void);

static volatile void	usage( void );
static void		trace( const char* , const char** );
static void		setup_doscall_handler( void );
static void		restore_doscall_handler( void );
static void		convert_slash_to_backslash( char* );


static char	*Myname;
extern void	New_handler( void );

handler	Old_handler;
FILE	*Stream;
int	Count = 0;


static volatile void	usage()
{
    fprintf( stderr , "DOS call tracer version 0.1 Copyright (C) 1991 K.Abe\n" );
    fprintf( stderr , "Usage: %s [-o file] command arg ...\n" , Myname );
    exit(1);
}


extern int	main( argc , argv )
int	argc;
char	*argv[];
{
    char	*command_name;
    char	**command_argv;
    char	*ofile = NULL;
    int		i;

    Myname = argv[0];
    if( argc <= 1 )
	usage();

    for( i = 1 ; i < argc && argv[i][0] == '-' ; i++ ) {
	switch( argv[i][1] ) {
	case 'o':
	    if( argv[i][2] )
		ofile = argv[i] + 2;
	    else
		ofile = argv[i++ + 1];
	    break;
	default:
	    usage();
	    break;
	}
    }
    if( ofile != NULL ) {
	if( ( Stream = fopen( ofile , "wt" ) ) == NULL ) {
	    perror( "fopen" );
	    exit(1);
	}
    } else
	Stream = stdout;

    Initialize_argument_information();
    command_name = argv[i];
    command_argv = &argv[i];
    convert_slash_to_backslash( command_name );
    trace( command_name , command_argv );
    return 0;
}


static void	trace( command_name , command_argv )
const char	*command_name;
const char	**command_argv;
{
    int		rc;

    setup_doscall_handler();
    rc = spawnvp( P_WAIT , command_name , command_argv );
    restore_doscall_handler();
    if( rc == -1 ) {
	perror( "spawnvp" );
	exit(1);
    }
    fprintf( Stream , "Exit code %d (%d System Calls)\n" , rc , Count );
}


static void	setup_doscall_handler()
{
    Old_handler = INTVCS( FLINE_TRAP_VECTOR , New_handler );
}


static void	restore_doscall_handler()
{
    long	usp;

    /* INTVCS( FLINE_TRAP_VECTOR , Old_handler ); do not work
       because New_handler changes FLINE_TRAP_VECTOR to New_handler after INTVCS().
       so, we must change FLINE_TRAP_VECTOR directly. */

    usp = SUPER(0);
    *(handler*)( FLINE_TRAP_VECTOR * 4 ) = Old_handler;
    SUPER( usp );
}


static void	convert_slash_to_backslash( command_name )
char	*command_name;
{
    for( ; *command_name ; command_name++ ) {
	if( *command_name == '/' )
	    *command_name = '\\';
    }
}
