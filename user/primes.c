#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime_filter(int fd) {
  int input = 0;
  int n = 0;
  int pid = 0;
  int fds[2] = {0, 0};

  while (1) {
    int len = read(fd, &input, sizeof(input));

    if (len <= 0) {
      break;
    }

    if (n == 0) {
      n = input;
      printf("prime %d\n", n);
    } else if (input % n != 0) {
      if (pid == 0) {
        pipe(fds);
        pid = fork();
        if (pid < 0) {
          fprintf(2, "fork error\n");
          close(fds[0]);
          close(fds[1]);
          break;
        } else if (pid == 0) {
          close(fd);
          close(fds[1]);
          prime_filter(fds[0]);
          return;
        } else {
          close(fds[0]);
        }
      }
      write(fds[1], &input, sizeof(input));
    }
  }
  close(fds[1]);
  close(fd);

  if (pid > 0) {
    wait((int*)0);
  }
}

int main(int argc, char *argv[]) {
  int fds[2];
  int i;
  int pid;

  if (pipe(fds) < 0) {
    fprintf(2, "pipe error\n");
    exit(1);
  }

  pid = fork();
  if (pid < 0) {
    fprintf(2, "fork error\n");
    exit(1);
  } else if (pid == 0) {
    close(fds[1]);
    prime_filter(fds[0]);
  } else {
    close(fds[0]);
    for (i = 2; i <= 35; i++) {
      write(fds[1], &i, sizeof(i));
    }
    close(fds[1]);
    wait((int *)0);
  }

  exit(0);
}
