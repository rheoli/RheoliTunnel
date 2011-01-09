
#include <stdio.h>

void gotoXY ( int _x, int _y )
{
  printf ( "\x01B[%d;%dH", _y, _x );
}

void putCharXY ( int _x, int _y, int _iOut )
{
  printf ( "\x01B[%d;%dH%c", _y, _x, (char)_iOut );
}

void putInversCharXY ( int _x, int _y, int _iOut )
{
  printf ( "\x01B[7m\x01B[%d;%dH%c\x01B[0m", _y, _x, (char)_iOut );
}

void cursorOn ( void )
{
  printf ( "\x01B[?25h" );
}

void cursorOff ( void )
{
  printf ( "\x01B[?25l" );
}

void inversOn ( void )
{
  printf ( "\x01B[7m" );
}

void inversOff ( void )
{
  printf ( "\x01B[0m" );
}

void blinkOn ( void )
{
  printf ( "\x01B[5m" );
}

void blinkOff ( void )
{
  printf ( "\x01B[0m" );
}

void clrScr ( void )
{
  printf ( "\x01B[2J\x01B[H" );
}

void putLDCharUR ( void )
{
  printf ( "\x01B(0\x06A\x01B(B" );
}

void putLDCharOR ( void )
{
  printf ( "\x01B(0\x06B\x01B(B" );
}

void putLDCharOL ( void )
{
  printf ( "\x01B(0\x06C\x01B(B" );
}

void putLDCharUL ( void )
{
  printf ( "\x01B(0\x06D\x01B(B" );
}

void putLDCharLM ( void )
{
  printf ( "\x01B(0\x074\x01B(B" );
}

void putLDCharRM ( void )
{
  printf ( "\x01B(0\x075\x01B(B" );
}

void putLDCharUM ( void )
{
  printf ( "\x01B(0\x076\x01B(B" );
}

void putLDCharOM ( void )
{
  printf ( "\x01B(0\x077\x01B(B" );
}

void putLDCharCurUp ( void )
{
  printf ( "\x01B(0\x02D\x01B(B" );
}

void putLDCharCurDown ( void )
{
  printf ( "\x01B(0\x02E\x01B(B" );
}

void putLDCharKR ( void )
{
  printf ( "\x01B(0\x06E\x01B(B" );
}

void putLDCharHL ( int _x, int _y, int _iAnz )
{
  int i;
  printf ( "\x01B(0" );
  printf ( "\x01B[%d;%dH", _y, _x );
  for ( i = 0; i < _iAnz; i++ )
    putchar ( '\x071' );
  printf ( "\x01B(B" );
}

void putLDCharVL ( int _x, int _y, int _iAnz )
{
  int i;
  printf ( "\x01B(0" );
  for ( i = 0; i < _iAnz; i++ )
    printf ( "\x01B[%d;%dH\x078", _y+i, _x );
  printf ( "\x01B(B" );
}

#define _PROGRAM

#ifdef _PROGRAM
void main ( void )
{
  int i;
  clrScr ();
  gotoXY ( 1, 1 );
  for ( i = 45; i < 47; i++ )
    printf ( "\x01B(0%c\x01B(B",i );
}
#endif
