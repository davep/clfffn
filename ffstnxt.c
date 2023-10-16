#include <extend.h>
#include "ffstnxt.ch"

/* Possible file modes. */
#define ATTR_READONLY  0x0001
#define ATTR_HIDDEN    0x0002
#define ATTR_SYSTEM    0x0004
#define ATTR_DIRECTORY 0x0010
#define ATTR_ARCHIVE   0x0020

typedef struct                  /* Findfirst/next() structure. */
{
    char     reserved[ 21 ];    /* Reserved data. */
    char     mode;              /* Mode of the entry. */
    unsigned time;              /* Time stamp of the entry. */
    unsigned date;              /* Date stamp of the entry. */
    long     size;              /* Size of the entry. */
    char     name[ 14 ];        /* Name of the entry. */
} find_struct;

/* BEWARE: Clipper internals */

extern void  __cdecl _bset( void *, char, unsigned );
extern int   __cdecl _lntoa( char far *, long, unsigned, unsigned );

/* You can find these two in _FFSTNXT.ASM */

extern int  __cdecl _cl_findfirst( char *, find_struct *, unsigned );
extern int  __cdecl _cl_findnext( find_struct * );

/* Local stuff. */

static int  EncodeAttr( char * );
static void DecodeAttr( int, char * );
static char *ZeroSpace( char * );

/*
 */

CLIPPER clFindFirs/*t*/( void )
{
    if ( PCOUNT && ISCHAR( 1 ) )
    {
        char        *pszSpec = _parc( 1 );
        int         iMode    = EncodeAttr( ISCHAR( 2 ) ? _parc( 2 ) : "" );
        find_struct find;

        _bset( (void *) &find, 0, sizeof( find_struct ) );

        if ( _cl_findfirst( pszSpec, &find, iMode ) )
        {
            _retclen( (unsigned char *) &find, sizeof( find_struct ) );
        }
        else
        {
            _retc( "" );
        }
    }
    else
    {
        _retc( "" );
    }
}

/*
 */

CLIPPER clFindNext( void )
{
    int iErr = -1;

    if ( PCOUNT && ISCHAR( 1 ) && _parclen( 1 ) == sizeof( find_struct ) )
    {
        find_struct *find = (find_struct *) _parc( 1 );

        if ( _cl_findnext( find ) )
        {
            _retclen( (unsigned char *) find, sizeof( find_struct ) );
        }
        else
        {
            _retc( "" );
        }
    }
    else
    {
        _retc( "" );
    }
}

/*
 */

CLIPPER clFindDeta/*ils*/( void )
{
    if ( PCOUNT && ISCHAR( 1 ) && _parclen( 1 ) == sizeof( find_struct ) )
    {
        find_struct *find = (find_struct *) _parc( 1 );
        int         iWhat = ( ISNUM( 2 ) ? _parni( 2 ) : CLFFN_NAME );

        switch ( iWhat )
        {
            case CLFFN_NAME :
                _retc( find->name );
                break;

            case CLFFN_DATE :
            {
                char buf[ 9 ];

                _lntoa( buf + 0, ( ( find->date & 0xFE00 ) >> 9 ) + 1980, 4, 0 );
                _lntoa( buf + 4,   ( find->date & 0x01E0 ) >> 5         , 2, 0 );
                _lntoa( buf + 6,     find->date & 0x001F                , 2, 0 );
                _retds( ZeroSpace( buf ) );

                break;
            }

            case CLFFN_TIME :
            {
                char buf[ 9 ];

                _lntoa( buf + 0, ( find->time & 0xF800 ) >> 11, 2, 0 ); buf[ 2 ] = ':';
                _lntoa( buf + 3, ( find->time & 0x07E0 ) >>  5, 2, 0 ); buf[ 5 ] = ':';
                _lntoa( buf + 6, ( find->time & 0x001F ) <<  1, 2, 0 );

                _retc( ZeroSpace( buf ) );

                break;
            }

            case CLFFN_ATTRIB :
            {
                char szAttr[ 6 ];

                DecodeAttr( find->mode, szAttr );
                _retc( szAttr );
                break;
            }

            case CLFFN_SIZE :
                _retnl( find->size );
                break;
        }
    }
    else
    {
        _ret();
    }
}

/*
 */

static int EncodeAttr( char *p )
{
    int iMode = 0;

    while ( *p )
    {
        switch ( *p )
        {
            case 'a' :
            case 'A' :
                iMode |= ATTR_ARCHIVE;
                break;

            case 'r' :
            case 'R' :
                iMode |= ATTR_READONLY;
                break;

            case 'h' :
            case 'H' :
                iMode |= ATTR_HIDDEN;
                break;

            case 's' :
            case 'S' :
                iMode |= ATTR_SYSTEM;
                break;

            case 'd' :
            case 'D' :
                iMode |= ATTR_DIRECTORY;
                break;
        }

        ++p;
    }

    return( iMode );
}

/*
 */

static void DecodeAttr( int iMode, char *p )
{
    if ( iMode & ATTR_READONLY )
    {
        *p++ = 'R';
    }

    if ( iMode & ATTR_HIDDEN )
    {
        *p++ = 'H';
    }

    if ( iMode & ATTR_SYSTEM )
    {
        *p++ = 'S';
    }

    if ( iMode & ATTR_DIRECTORY )
    {
        *p++ = 'D';
    }

    if ( iMode & ATTR_ARCHIVE )
    {
        *p++ = 'A';
    }

    *p = 0;
}

/*
 */

static char *ZeroSpace( char *s )
{
    int i;

    for ( i = 0; s[ i ]; i++ )
    {
        if ( s[ i ] == ' ' )
        {
            s[ i ] = '0';
        }
    }

    return( s );
}
