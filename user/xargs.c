#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  int n = -1;
  int i = 1;
  int pid;
  int param = 0;
  int ret;
  char *params[MAXARG] = {0};
  char *arg;
  char buf[512];
  char c;

  while (argv[i] != 0) {
    arg = argv[i];
    if (n == -1 && strcmp(arg, "-n") == 0) {
      i++;
      n = atoi(argv[i]);
    } else {
      params[param++] = arg;
    }
    i++;
  }

  i = 0;
  while ((ret = read(0, &c, 1)) == 1 || i != 0) {
    buf[i++] = c;
    if (i == n || c == '\n' || ret <= 0) {
      if (c == '\n') {
        buf[--i] = 0;
      } else {
        buf[i] = 0;
      }
      params[param] = buf;
      params[param+1] = 0;
      pid = fork();
      if (pid == 0) {
        exec(params[0], params);
        fprintf(2, "exec %s failed\n", params[0]);
      } else if (pid > 0) {
        wait((int*)0);
        i = 0;
      }
    }
  }

  exit(0);
}
