#include <sys/types.h>
#include <sys/time.h>
#include <sys/termios.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#include <unistd.h>
#include <stdio.h>

static struct termios sav;

void restore_tty(void)
{
   (void)tcsetattr(0, TCSADRAIN, &sav);
}

void init_tty(void)
{
   struct termios t;

   (void)tcgetattr(0, &t);
   sav = t;
   t.c_lflag &= ~(ICANON);
   t.c_cc[VMIN] = 1;
   t.c_cc[VTIME] = 0;
   (void)tcsetattr(0, TCSADRAIN, &t);
}

int kbhit(void)
{
   struct timeval tv;
   fd_set ifd;

   FD_ZERO(&ifd);
   FD_SET(0, &ifd);
   tv.tv_sec = 0;
   tv.tv_usec = 0;

   return select(1, &ifd, 0, 0, &tv) == 1;
}

int getch(void)
{
   char c;

   fflush(stdout);
   if (read(0, &c, 1) == 1)
      return c;
   restore_tty();
   exit(1);
}
