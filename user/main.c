#include "sys.h"

int main() {
  char s[] = ".";
  for (int j = 0; j < 10; j++) {
    write("PID=");
    int pid = getpid();
    s[0] = '0' + pid;
    write(s);
    write("\n");
    for (int i = 0; i < 100000000; i++)
      ;
  }
  char s2[] = "PID=? EXIT\n";
  int pid = getpid();
  s2[4] = '0' + pid;
  write(s2);
  return 0;
}
