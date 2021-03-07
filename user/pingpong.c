#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int p2cfds[2];
  int c2pfds[2];
  int pid;
  char c;

  pipe(p2cfds);
  pipe(c2pfds);

  pid = fork();
  if (pid == 0) {
    close(p2cfds[1]);
    close(c2pfds[0]);

    read(p2cfds[0], &c, 1);
    printf("%d: received ping\n", getpid());
    write(c2pfds[1], "#", 1);

    close(p2cfds[0]);
    close(c2pfds[1]);
  } else {
    close(p2cfds[0]);
    close(c2pfds[1]);

    write(p2cfds[1], "#", 1);
    read(c2pfds[0], &c, 1);
    printf("%d: received pong\n", getpid());

    close(p2cfds[1]);
    close(c2pfds[0]);

    wait((int *)0);
  }

  exit(0);
}


