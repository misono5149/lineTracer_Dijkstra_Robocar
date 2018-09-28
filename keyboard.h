#ifndef _KEY_BOARD_
#define _KEY_BOARD_

  #include <stdio.h>
  #include <termios.h>
   
  void init_keyboard();
  void close_keyboard() ;
  
  int kbhit();
  int readch();

  
  
#endif  
