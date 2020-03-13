#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char const *argv[]) {

  // 3.a membuat direktori di /home/el/modul2 yaitu indomie
  // dan 5 detik kemudian bikin direktori sedaap
  pid_t child_id_1, child_id_2, child_id_3, child_id_4,
        child_id_5, child_id_6;
  int status;

  child_id_1 = fork();

  if (child_id_1 == 0){
    // buat bikin folder
    char *argv[4] = {"mkdir", "-p", "/home/el/modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
  }

  sleep(5);

  child_id_2 = fork();

  if (child_id_2 == 0){
    char *argv[4] = {"mkdir", "-p", "/home/el/modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
  }

  // 3.b ekstrak file jpg.zip dari direktori /home/el/modul2
  while((waitpid(child_id_2, &status, 0)) > 0);

  child_id_3 = fork();

  if (child_id_3 == 0){
    char *argv[5] = {"unzip", "jpg.zip", "-d", "/home/el/modul2", NULL};
    execv("/usr/bin/unzip", argv);
  }

  // 3.c hasil ekstrak dari direktori /home/el/modul2/jpg dipindah
  //     file dipindah ke sedaap
  //     direktori dipindah ke indomie

  while((waitpid(child_id_3, &status, 0)) > 0);

  child_id_4 = fork();

  if (child_id_4 == 0){
    char *argv[13] = {"find", "/home/el/modul2/jpg/", "-not", "-type", "d",
                     "-exec", "/bin/mv", "-t", "/home/el/modul2/sedaap",
                     "--", "{}", "+", NULL};
    execv("/usr/bin/find", argv);
  }

  while((waitpid(child_id_4, &status, 0)) > 0);

  child_id_5 = fork();

  if (child_id_5 == 0){
    char *argv[16] = {"find", "/home/el/modul2/jpg/", "-maxdepth", "1",
                      "-mindepth", "1", "-type", "d", "-exec", "/bin/mv",
                      "-t", "/home/el/modul2/indomie", "--", "{}", "+", NULL};
    execv("/usr/bin/find", argv);
  }

  // 3.d untuk tiap direktori di indomie, buat dua file kosong, yaitu coba1.txt
  // dan 3 detik kemudian bikin coba2.txt

  else{
    while((wait(&status)) > 0);

    DIR *dir = opendir("/home/el/modul2/indomie");
    struct dirent *entry, *subentry;
    int files = 0;

    FILE *file_1, *file_2;

    if (dir == NULL){
      perror("Unable to open\n");
      return(1);
    }

    while((entry = readdir(dir))){
     if(entry->d_name[0]!='.'){
        char dirname_1[100] = "/home/el/modul2/indomie/";

        char txt_1[10] = "/coba1.txt";
        strncat(dirname_1, entry->d_name, strlen(entry->d_name));
        strncat(dirname_1, txt_1, 10);

        file_1 = fopen(dirname_1, "a");
        if (file_1 == NULL)
        printf("Error creating file!\n");
        fclose(file_1);

        sleep(3);

        char dirname_2[100] = "/home/el/modul2/indomie/";
        char txt_2[] = "/coba2.txt";
        strncat(dirname_2, entry->d_name, strlen(entry->d_name));
        strncat(dirname_2, txt_2, 10);

        file_2 = fopen(dirname_2, "a");
        if (file_2 == NULL)
        printf("Error creating file!\n");
        fclose(file_2);

      }
    }

    closedir(dir);

  }

  return 0;
}
