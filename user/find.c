#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(const char *path, const char *filename) {
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512];
  char *p;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type != T_DIR) {
    fprintf(2, "find: %s is not a director\n", path);
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0) {
      continue;
    }

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(stat(buf, &st) < 0){
      printf("find: cannot stat %s\n", buf);
      continue;
    }
    switch (st.type) {
      case T_DIR:
        if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
          find(buf, filename);
        }
        break;

      case T_FILE:    // fallthrough
      case T_DEVICE:
        if (strcmp(de.name, filename) == 0) {
          printf("%s\n", buf);
        }
        break;
    }
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: %s <directoy> <filename>\n", argv[0]);
    exit(0);
  }

  find(argv[1], argv[2]);

  exit(0);
}
