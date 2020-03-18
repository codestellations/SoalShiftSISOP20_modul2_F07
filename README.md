# SoalShiftSISOP20_modul2_F07
Pengerjaan praktikum mata kuliah Sistem Operasi Informatika ITS 2020
- Erlinda Argyanti Nugraha (05111840000017)
- Lii'zza Aisyah Putri Sulistio (05111840000073)

## Soal 1
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan ketentuan sebagai berikut:
 a. Program menerima 4 argumen berupa:
   i. Detik: 0-59 atau * (any value)
   ii. Menit: 0-59 atau * (any value)
   iii. Jam: 0-23 atau * (any value)
   iv. Path file .sh
  b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai
  c. Program hanya menerima 1 config cron
  d. Program berjalan di background (daemon)
  e. Tidak boleh menggunakan fungsi system()
Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap detik pada jam 07:34.

Kendala: kesulitan mengecek apakah bash sudah dijalankan dengan benar oleh program.
> [source code program](https://github.com/codestellations/SoalShiftSISOP20_modul2_F07/blob/master/soal1/soal1.c)

### Mengecek banyaknya argumen yang dimasukkan dan progam hanya menerima 1 config cron
Berikut merupakan code untuk mengecek apakah banyaknya argumen yang dimasukkan sudah sesuai atau tidak. Jika argumen yang dimasukkan tidak samadengan 5 maka program akan ke luar dan mengeluarkan pesan mengapa jumlah argumen yang dimasukkan tidak sesuai. Pada main program, argc digunakan untuk menyimpan banyaknya argument yang dimasukkan oleh user.
~~~
if(argc != 5) /*karena argumen yg dibutuhkan 4*/
    {
        invalidArg();
        printf("Only 4 arguments needed, consists of: seconds | minutes | hour | pathfile\n");
        exit(EXIT_FAILURE);
    }
    int arguments[4];
~~~

### Mengecek tipe data yang dimasukkan pada Cron dan mengkonversi argumen ke variabel
Program akan menjalankan looping sebanyak argumen yang dimasukkan dan melakukan pengecekan dengan 2 fungsi.
1. Fungsi untuk mengecek apakah input merupakan angka:
~~~
int checkNum(char arg[])
{
    int i;
    for(i = 0; i < strlen(arg); i++)
    {
        if(arg[i] < '0' || arg[i] > '9')
        {
            return 0; /*jika input yg dimasukkan bukan angka (atau asterisk)*/
        }
    }
    return 1;
}
~~~


2. Fungsi untuk mengecek apakah input merupakan asterisk:
~~~
int checkAst(char arg[])
{
    if(strlen(arg) == 1)
    {
        if(arg[0] == '*')
        {
            return 1;
        }
    }
    return 0; /*jika input yg dimasukkan bukan asterisk (atau angka)*/
}
~~~
Jika input yang dimasukkan pada cron bukan berupa angka maupun asterisk program akan ke luar dan memunculkan pesan bahwa input yang dimasukkan invalid. Namun, jika input yang dimasukkan benar, maka program akan mengkonversi argumen yang dimasukkan ke dalam variabel sec, min, hr. 
~~~
for(i = 1; i < 4; i++) /*cek input argument untuk cron*/
{
  if(checkNum(argv[i]))
  {
       arguments[i] = atoi(argv[i]); //mengkonversi argument ke hr, min, sec
       if(arguments[i] < 0) invalidArg();
  }
  else if(checkAst(argv[i]))
  {
       arguments[i] = -1;
  }
  else
  {
       invalidArg();
       exit(EXIT_FAILURE);
  }   
}
~~~

### Mengecek ketentuan penulisan detik, menit, jam pada Cron
Berikut merupakan code untuk mengecek apakah penulisan detik, menit, dan jam sudah sesuai dengan ketentuan soal. Jika input yang dimasukkan user tidak sesuai, maka program akan mengeluarkan pesan jika input yang dimasukkan tidak sesuai dengan ketentuan.
~~~
/*mengecek ketentuan input apakah sudah sesuai atau belum*/
if(sec < -1 || sec > 59 || min < -1 || min > 59 || hr < -1 || hr > 23)
{
    invalidArg();
    printf("Input format: seconds (0 - 59) | minutes (0 - 59) | hour (0 - 23)\n");
    exit(EXIT_FAILURE);
}
~~~

### Program berjalan di background (daemon)
Code untuk daemon diambil dari template daemon pada modul 2 yang diubah agar sesuai dengan permintaan soal. Berikut merupakan code daemon yang digunakan:
~~~
 /*template Daemon*/
    pid_t pid, sid; //variabel untuk menyimpan PID
    pid = fork(); //menyimpan PID
    if(pid < 0) //keluar saat fork gagal
    {
        exit(EXIT_FAILURE);
    }
    if(pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    umask(0);
    sid = setsid();
       
    if(sid < 0)
    {
        exit(EXIT_FAILURE);
    }
    
    /*menutup file descriptor standar*/
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    while(1) /*loop utama*/
    {
        time_t raw = time(NULL);
        struct tm *currTime = localtime(&raw);
        
        /*current time*/
        curr_sec = currTime->tm_sec;
        curr_min = currTime->tm_min;
        curr_hr = currTime->tm_hour;
        
        if((curr_sec == arguments[1] || arguments[1] == -1) && (curr_min == arguments[2] || arguments[2] == -1 ) && (curr_hr == arguments[3] || arguments[3] == -1))
        {
            pid_t child_id;
            child_id = fork();
            int status;
            
            if(child_id == 0)
            {
                char *arg_exec[] = {"bash", argv[4], NULL};
                execv("/bin/bash", arg_exec);
            }
            else
            {
                while((wait(&status)) > 0);
            }
        }
        sleep(1);
    }
~~~

Input
<img width="836" alt="Screen Shot 2020-03-18 at 20 40 37" src="https://user-images.githubusercontent.com/58472359/76967088-7c802480-6959-11ea-9cb2-623ae8d71c00.png">
Bash akan dijalankan setiap 15 menit sekali.
> [bash yang digunakan](https://github.com/codestellations/SoalShiftSISOP20_modul2_F07/blob/master/soal1/password.sh)

Output
<img width="915" alt="Screen Shot 2020-03-18 at 20 55 42" src="https://user-images.githubusercontent.com/58472359/76968147-fbc22800-695a-11ea-8e33-940f1b8646a2.png">
> hasil dari dijalankannya bash tersebut disimpan dalam file [straykids.txt](https://github.com/codestellations/SoalShiftSISOP20_modul2_F07/blob/master/soal1/straykids.txt)


---

## Soal2
Inisiasi awal adalah dengan membuat program daemon yang akan berjalan di background.
### 2.a Membuat folder khusus yang di dalamnya membuat program c yang membuat folder bernama [timestamp] tiap 30 detik.
berikut adalah fungsi untuk membuat string berisi timestamp dengan nilai waktu yang didapat dari waktu sistem (local time).
~~~
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
~~~

setelah itu, masuk ke loop `while`.
~~~
strcpy(folder, times());
strcat(dirname, folder);
~~~
membuat string *dirname* berisi path direktori yang akan dibuat, yaitu `/home/el/khusus/[timestamp]`.
~~~
if(child_id_2 == 0){
  fprintf(stdout, "child 2 running %d\n", getpid());
  char *argv[4] = {"mkdir", "-p", dirname, NULL};
  execv("/bin/mkdir", argv);
}
~~~
child yang dibuat untuk membuat direktori dengan format string *dirname* dan ditaruh di folder `khusus`.

### 2.b Tiap folder diisi 20 gambar dari https://picsum.photos yang didownload tiap 5 detik dengan ukuran (t%1000)+100 dan dinamakan [timestamp]
~~~
unsigned long epoch = time(NULL),
              size = (epoch%1000)+100;
sprintf(filesize, "%lu", (epoch%1000)+100);

char url[100] = "https://picsum.photos/";
strcat (url, filesize);
~~~
fungsi untuk mendapatkan nilai epoch dan menyimpannya ke dalam string *filesize* supaya dapat digabungkan dengan string *url* tempat mendownload untuk mendapatkan ukuran yang diinginkan.

~~~
for (i = 0; i < 20; i++) {
.
.
.
  child_id = fork();
  if (child_id == 0) {
    char *argv[7] = {"wget", url, "-O", fordir, NULL};
    execv("/usr/bin/wget", argv);
  }
  sleep(5);
}
~~~
loop untuk mendownload 20 file dengan menggunakan command `wget` di dalam folder `/home/el/khusus/[timestamp]/[timestamp].jpg` dengan jeda 5 detik dalam tiap download.


### 2.c Setelah terisi 20 gambar, folder dizip dan folder original dihapus
~~~
strcpy(zipname, folder);
strcat(zipname, ".zip");
~~~
membuat string *zipname* yang berisi `/home/el/khusus/[timestamp]/[timestamp].zip`.

~~~
child_id_4 = fork();

if (child_id_4 == 0) {
  char *argv[7] = {"zip", "-rm", zipname, folder, NULL};
  execv("/usr/bin/zip", argv);
~~~
membuat child untuk menzip folder yang bersangkutan.
- `-r` digunakan untuk menzip rekursif untuk semua file di dalam folder.
- `-m` digunakna untuk menghapus folder awal setelah folder selesai dizip.

~~~
    sleep(30);
~~~
sleep untuk jeda 30 detik dalam membuat folder baru di dalam while.

### 2.d Generate program killer yang siap di run
~~~
  FILE * killer;
  killer = fopen("killer.sh", "w");
~~~
membuat file `killer.sh` dengan FILE.

### 2.e Program utama mempunyai MODE_A dan MODE_B yang berguna dalam meng-kill program tersebut
~~~
  if(argc == 2){
    if(strcmp(argv[1], "-a") == 0){
      fprintf(killer, "#!/bin/bash\nkillall soal2\n");
      fprintf(killer, "rm $0");
    }
    else if(strcmp(argv[1], "-b") == 0){
      fprintf(killer, "#!/bin/bash\nkill %d\n", getpid() + 1);
      fprintf(killer, "rm $0");
    }
    else{
      printf("Invalid command. Use -a or -b.\n");
      exit(0);
    }
  }

  else{
    printf("Invalid command. Insert only 2 arguments.\n");
    exit(0);
  }

  fclose(killer);
~~~
- *MODE_A* akan membunuh program utama secara paksa, di mana semua pekerjaan yang sedang dilakukan akan diberhentikan pula.
- *MODE_B* akan membunuh program utama secara lebih halus, di mana pekerjaan mendownload dan menzip folder yang sudah (dan belum) berjalan akan tetap berjalan sampai folder dizip dan dihapus, namun tidak ada lagi folder yang akan terbuat.
- `rm $0` berfungsi untuk menghapus file `killer.sh` tersebut setelah menjalankan tugasnya.

Kendala :
pada poin ini, baru MODE_A saja yang dapat berjalan. untuk MODE_B, program belum bisa berhenti total, melainkan hanya proses paling awal yang sudah berjalan saja.

---

## Soal 3
file pendukung dalam soal ini adalah file `jpg.zip` yang disertakan di dalam folder soal3.
### 3.a Membuat direktori di /home/[user]/modul2 yaitu *indomie* dan 5 detik kemudian membuat direktori lain yaitu *sedaap*
~~~
  child_id_1 = fork();

  if (child_id_1 == 0){
    // buat bikin folder
    char *argv[4] = {"mkdir", "-p", "/home/el/modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
  }
  
  sleep(5);
~~~
membuat child untuk menjalankan fungsi `mkdir` untuk membuat direktori *indomie* sesuai path yang diminta. sleep digunakan untuk memberi jeda 5 detik sebelum menjalankan fungsi selanjutnya

~~~
  child_id_2 = fork();

  if (child_id_2 == 0){
    char *argv[4] = {"mkdir", "-p", "/home/el/modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
  }
~~~
membuat child untuk menjalankan fungsi `mkdir` untuk membuat direktori *sedaap* sesuai path yang diminta.

### 3.b Mengekstrak file *jpg.zip* dari direktori /home/[user]/modul2
~~~
  while((waitpid(child_id_2, &status, 0)) > 0);

  child_id_3 = fork();

  if (child_id_3 == 0){
    char *argv[5] = {"unzip", "jpg.zip", "-d", "/home/el/modul2", NULL};
    execv("/usr/bin/unzip", argv);
  }
~~~
setelah child dari 3.a selesai dijalankan, membuat child baru untuk mengunzip `jpg.zip` dengan fungsi `unzip` dan disimpan di `/home/[user]/modul2`.

### 3.c Hasil ekstrak tadi dipindah ke direktori *sedaap* (untuk file) dan direktori *indomie* (untuk folder)
~~~
  while((waitpid(child_id_3, &status, 0)) > 0);

  child_id_4 = fork();

  if (child_id_4 == 0){
    char *argv[13] = {"find", "/home/el/modul2/jpg/", "-not", "-type", "d",
                     "-exec", "/bin/mv", "-t", "/home/el/modul2/sedaap",
                     "--", "{}", "+", NULL};
    execv("/usr/bin/find", argv);
  }
~~~
child untuk memindahkan file ke dalam direktori *sedaap*.
- `find /home/el/modul2/jpg/ -not -type d` mencari di dalam direktori *jpg* untuk semua item yang bukan direktori.
- `-exec /bin/mv -t /home/el/modul2/sedaap -- {} +` memindahkan hasil *find* tadi ke dalam direktori *sedaap*.

~~~
  while((waitpid(child_id_4, &status, 0)) > 0);

  child_id_5 = fork();

  if (child_id_5 == 0){
    char *argv[16] = {"find", "/home/el/modul2/jpg/", "-maxdepth", "1",
                      "-mindepth", "1", "-type", "d", "-exec", "/bin/mv",
                      "-t", "/home/el/modul2/indomie", "--", "{}", "+", NULL};
    execv("/usr/bin/find", argv);
  }
~~~
child untuk memindahkan file ke dalam direktori *indomie*.
- `find /home/el/modul2/jpg/ -maxdepth 1 -mindepth 1 -type d` mencari di dalam direktori *jpg* untuk semua item yang merupakan direktori. *maxdepth* supaya tidak mencari di dalam subdirectory jpg, dan *mindepth* supaya tidak mencari di luar direktori jpg.
- `-exec /bin/mv -t /home/el/modul2/indomie -- {} +` memindahkan hasil *find* tadi ke dalam direktori *indomie*.


### 3.d Untuk tiap direktori di *indomie*, buat dua file yaitu *coba1.txt* dan 3 detik kemudian *coba2.txt*.
untuk poin ini, kami menggunakan DIR untuk mengakses direktori, dan FILE untuk membuat file baru.
~~~
    DIR *dir = opendir("/home/el/modul2/indomie");
    struct dirent *entry, *subentry;

    FILE *file_1, *file_2;
    
    while((entry = readdir(dir))){
     .
     .
     .
    }

    closedir(dir);
~~~
loop untuk membaca isi dari direktori *indomie* dan menutup kembali direktori setelah loop selesai.

~~~
if(entry->d_name[0]!='.')
~~~
untuk tidak membaca direktori `.` dan `..`

~~~
  char txt_1[10] = "/coba1.txt";
  strncat(dirname_1, entry->d_name, strlen(entry->d_name));
  strncat(dirname_1, txt_1, 10);

  file_1 = fopen(dirname_1, "a");
  if (file_1 == NULL)
  printf("Error creating file!\n");
  fclose(file_1);

  sleep(3);
~~~
untuk membuat file `coba1.txt`, dan memberi jeda 3 detik sebelum menjalankan selanjutnya.

~~~
  char dirname_2[100] = "/home/el/modul2/indomie/";
  char txt_2[] = "/coba2.txt";
  strncat(dirname_2, entry->d_name, strlen(entry->d_name));
  strncat(dirname_2, txt_2, 10);

  file_2 = fopen(dirname_2, "a");
  if (file_2 == NULL)
  printf("Error creating file!\n");
  fclose(file_2);
}
~~~
untuk membuat file `coba2.txt`.

---

#### Input soal 3
![Screenshot from 2020-03-18 22-09-13](https://user-images.githubusercontent.com/57877040/76979073-118b1980-696a-11ea-896f-b09be30e38e9.png)

#### Output folder `modul2`
![Screenshot from 2020-03-18 22-09-29](https://user-images.githubusercontent.com/57877040/76979760-fcfb5100-696a-11ea-8da8-fc54711697e4.png)

#### Output folder `sedaap`
![Screenshot from 2020-03-18 22-10-30](https://user-images.githubusercontent.com/57877040/76979807-0be20380-696b-11ea-823e-e51b3d1e10e6.png)

#### Output folder `indomie`
![Screenshot from 2020-03-18 22-10-43](https://user-images.githubusercontent.com/57877040/76979833-14d2d500-696b-11ea-9220-7e9d2fb5c904.png)

![Screenshot from 2020-03-18 22-10-59](https://user-images.githubusercontent.com/57877040/76979851-19978900-696b-11ea-8b94-e53d0dbb8fbc.png)

![Screenshot from 2020-03-18 22-15-34](https://user-images.githubusercontent.com/57877040/76979881-24521e00-696b-11ea-8aa7-92e5abf14ba4.png)

