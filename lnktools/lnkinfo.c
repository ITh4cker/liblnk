/*
 * Shows information obtained from a Windows Shortcut (LNK) File
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include <stdio.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

/* If libtool DLL support is enabled set LIBLNK_DLL_IMPORT
 * before including liblnk.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBLNK_DLL_IMPORT
#endif

#include <liblnk.h>

#include <libsystem.h>

#include "lnkoutput.h"

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use lnkinfo to determine information about a Windows Shortcut (LNK) File.\n\n" );

	fprintf( stream, "Usage: lnkinfo [ -hvV ] source\n\n" );

	fprintf( stream, "\tsource: the source file\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Prints file information
 * Returns 1 if successful or -1 on error
 */
int lnkinfo_file_info_fprint(
     FILE *stream,
     liblnk_file_t *file,
     liblnk_error_t **error )
{
	static char *function = "lnkinfo_file_info_fprint";

	if( stream == NULL )
	{
		fprintf(
		 stderr,
		 "%s: invalid stream.\n",
		 function );

		return( -1 );
	}
	if( file == NULL )
	{
		fprintf(
		 stderr,
		 "%s: invalid file.\n",
		 function );

		return( -1 );
	}
	fprintf(
	 stream,
	 "Windows Shortcut information:\n" );

	fprintf(
	 stream,
	 "\n" );

	return( 1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER_T )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	liblnk_error_t *error         = NULL;
	liblnk_file_t *lnk_file       = NULL;
	libsystem_character_t *source = NULL;
	char *program                 = "lnkinfo";
	libsystem_integer_t option    = 0;
	int verbose                   = 0;

	libsystem_notify_set_stream(
	 stderr,
	 NULL );
	libsystem_notify_set_verbose(
	 1 );

        if( libsystem_initialize(
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liberror_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	lnkoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBSYSTEM_CHARACTER_T_STRING( "hvV" ) ) ) != (libsystem_integer_t) -1 )
	{
		switch( option )
		{
			case (libsystem_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %s\n",
				 argv[ optind ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libsystem_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libsystem_integer_t) 'v':
				verbose = 1;

				break;

			case (libsystem_integer_t) 'V':
				lnkoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind ];

	libsystem_notify_set_verbose(
	 verbose );
	liblnk_notify_set_stream(
	 stderr,
	 NULL );
	liblnk_notify_set_verbose(
	 verbose );

	if( liblnk_file_initialize(
	     &lnk_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize liblnk file.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liblnk_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	if( liblnk_file_open(
	     lnk_file,
	     source,
	     LIBLNK_OPEN_READ,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Error opening file: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liblnk_error_free(
		 &error );

		liblnk_file_free(
		 &lnk_file,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( lnkinfo_file_info_fprint(
	     stdout,
	     lnk_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print file information.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liblnk_error_free(
		 &error );

		liblnk_file_free(
		 &lnk_file,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( liblnk_file_close(
	     lnk_file,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Error closing file: %" PRIs_LIBSYSTEM ".\n",
		 argv[ optind ] );

		libsystem_notify_print_error_backtrace(
		 error );
		liblnk_error_free(
		 &error );

		liblnk_file_free(
		 &lnk_file,
		 NULL );

		return( EXIT_FAILURE );
	}
	if( liblnk_file_free(
	     &lnk_file,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free liblnk file.\n" );

		libsystem_notify_print_error_backtrace(
		 error );
		liblnk_error_free(
		 &error );

		return( EXIT_FAILURE );
	}
	return( EXIT_SUCCESS );
}
