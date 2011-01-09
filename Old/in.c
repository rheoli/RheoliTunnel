#include <stdio.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/fcntl.h>
 
static struct termios tOrigTTY, tNewTTY;

void resetTerm ( void )
{
  if ( tcsetattr (0, TCSADRAIN, &tOrigTTY) == -1 )  {
    printf ( "Error in tcsetattr\n" );
    exit ( 1 );
  }
}
 
void initTerm ( void )
{
  if ( tcgetattr(0, &tOrigTTY) == -1 )  {
    fprintf ( stderr, "Error in tcgetattr\n" );
    exit ( 1 );
  }
  memcpy ( &tNewTTY, &tOrigTTY, sizeof(struct termios) );
  tNewTTY.c_lflag    &= ~ICANON;
  tNewTTY.c_lflag    &= ~ECHO;
  tNewTTY.c_cc[VMIN]  = 1;
  tNewTTY.c_cc[VTIME] = 0;
  if ( tcsetattr (0, TCSADRAIN, &tNewTTY) == -1 )  {
    fprintf ( stderr, "Error in tcsetattr\n" );
    exit ( 1 );
  }
}

int waitForChar ( void )
{
  fd_set         fdMask;
  struct timeval tvWait;

  tvWait.tv_sec  = 1;
  tvWait.tv_usec = 0;

  FD_SET(0, &fdMask);

  if ( select ( 32, &fdMask, 0, 0, &tvWait ) == 0 )  {
    return ( 0 );
  }
  return ( 1 );
}

int getCharInBuff ( void )
{
  int nb;
  if ( waitForChar() != 0 )  {
    ioctl ( 0, FIONREAD, &nb );
    return ( nb );
  }
  return ( 0 );  
}

int main() {
  char c;
  initTerm();
  while ( (c=getCharInBuff())!='a' ) {
    //printf("Loop");
    printf("%c", c);
  }
  resetTerm();
}

