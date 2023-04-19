#include "sys.h"

int main() {
  char s[] = ".";
  char s3[] = "Fork return ?\n";
  int fret = 0;
  // int fret = fork();
  s3[12] = '0' + fret;
  write(s3);
  for (int j = 0; j < 5; j++) {
    if (fret != 0) {
      write("I'm parent, PID=");
    } else {
      write("I'm child, PID=");
    }
    int pid = getpid();
    s[0] = '0' + pid;
    write(s);
    write("\n");
    for (int i = 0; i < 500000; i++)
      ;
  }

  char s2[] = "PID=? EXIT\n";
  int pid = getpid();
  s2[4] = '0' + pid;
  write(s2);
  return 0;
}
