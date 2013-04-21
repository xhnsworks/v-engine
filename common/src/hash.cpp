#include "pch.h"
#include "hash.h"
uint32 ELFHash( char   * str)
{
    uint32  hash  =   0 ;
    uint32  x     =   0 ;

    while  ( * str)
    {
        hash  =  (hash  <<   4 )  +  ( * str ++ );
        if  ((x  =  hash  &   0xF0000000L )  !=   0 )
        {
            hash  ^=  (x  >>   24 );
            hash  &=   ~ x;
        }
    }

    return  (hash  &   0x7FFFFFFF );
}

uint32 calc_hashnr(const char *key, uint32 length)
{
	if (!length)
		return 0;
    register uint32 nr=1, nr2=4;
    while (length--)
    {
        nr^= (((nr & 63)+nr2)*((uint32) (uint8) *key++))+ (nr << 8);
        nr2+=3;
    }
    return((uint32) nr);
}
