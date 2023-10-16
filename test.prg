#include "ffstnxt.ch"

Function Main()
Local hFind

   Set Cent On
   Set Date Japan

   scroll()

   hFind := clFindFirst( "*.*" )

   Do While !empty( hFind )

      ? padr( clFindDetails( hFind, CLFFN_NAME ),   14 ),;
        padr( clFindDetails( hFind, CLFFN_DATE ),   10 ),;
        padr( clFindDetails( hFind, CLFFN_TIME ),    8 ),;
        padr( clFindDetails( hFind, CLFFN_ATTRIB ),  5 ),;
        padl( clFindDetails( hFind, CLFFN_SIZE ),   20 )

      hFind := clFindNext( hFind )

   EndDo

Return( NIL )
