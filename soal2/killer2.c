#include <unistd.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  if(argc == 2){
    if(strcmp(argv[1], "-a") == 0){
      char *argv[3] = {"killall", "soal2", NULL};
      execv("usr/bin/killall", argv);
    }
  }
  return 0;
}
