#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <wait.h>

char time_name[100];

char* times(){
  int hours, minutes, seconds, day, month, year;
  time_t T = time(NULL);
  struct tm tm = *localtime(&T);

  hours = tm.tm_hour;
  minutes = tm.tm_min;
  seconds = tm.tm_sec;
  day = tm.tm_mday;
  month = tm.tm_mon + 1;
  year = tm.tm_year + 1900;

  sprintf(time_name, "%02d-%02d-%d_%02d:%02d:%02d",
          day, month, year, hours, minutes, seconds);

  return time_name;
}

int main(int argc, char const *argv[]) {
  pid_t sid, pid, child_id_1, child_id_2, child_id_3, child_id;

  pid = fork();

  if(pid < 0){
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/el/khusus")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1){
    int hours, minutes, seconds, day, month, year;
    int files = 0;
    int status;
    char dirname[100] = "/home/el/khusus/", filesize[100],
         fordir[100], zipname[100];

    // 2.a membuat folder khusus, di dalamnya membuat program c
    //     yang membuat folder bernama [timestamp] per 30 detik
    strcat(dirname, times());

    child_id_1 = fork();
    if(child_id_1 == 0){
      char *argv[4] = {"mkdir", "-p", dirname, NULL};
      execv("/bin/mkdir", argv);
    }

    // 2.b tiap folder diisi 20 gambar dari https://picsum.photos/
    //     didownload tiap 5 detik dengan ukuran (t%1000)+100 dan
    //     dinamakan [timestamp]
    unsigned long epoch = time(NULL),
                  size = (epoch%1000)+100;
    sprintf(filesize, "%lu", (epoch%1000)+100);

    while((waitpid(child_id_1, &status, 0)) > 0);

    child_id_2 = fork();
    if (child_id_2 == 0) {
      int i;
      for (i = 0; i < 20; i++) {
        unsigned long epoch = time(NULL),
                      size = (epoch%1000)+100;
        sprintf(filesize, "%lu", (epoch%1000)+100);

        char url[100] = "https://picsum.photos/";
        strcat (url, filesize);

        strcpy(fordir, dirname);
        strcat(fordir, "/");
        strcat(fordir, times());

        child_id = fork();
        if (child_id == 0) {
          // char *argv[19] = {"wget", "-O", fordir, url, "2", ">", "&", "1",
          //                   "|", "tee", "-a", "/home/el/khusus/wget.log", NULL};
          // execv("/usr/bin/wget", argv);

          char *argv[7] = {"wget", url, "-O", fordir, NULL};
          execv("/usr/bin/wget", argv);

          // FILENYA BUKAN FOTO ANJ
          // char *argv[5] = {"curl", "-o", fordir, url, NULL};
          // execv("/usr/bin/curl", argv);
        }
        sleep(5);
      }

      // 2.c setelah terisi 20, folder dizip dan didelete
      strcpy(zipname, dirname);
      strcat(zipname, ".zip");

      child_id_3 = fork();

      if (child_id_3 == 0) {
        char *argv[7] = {"zip", "-rm", zipname, dirname, NULL};
        execv("/usr/bin/zip", argv);
      }
    }

    // 2.d generate program "killer" yang siap di run

    // 2.e program utama punya mode -a untuk force stop kalau di kill
    //     dan mode -b untuk berjalan sampai selesai kalau di kill

    sleep(30);
  }

  return 0;
}
