# SoalShiftSISOP20_modul2_F07
Pengerjaan praktikum mata kuliah Sistem Operasi Informatika ITS 2020

## Soal 1
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan ketentuan sebagai berikut:
a. Program menerima 4 argumen berupa:
i. Detik: 0-59 atau * (any value)
ii. Menit: 0-59 atau * (any value)
iii. Jam: 0-23 atau * (any value)
iv. Path file .sh
b. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak
sesuai
c. Program hanya menerima 1 config cron
d. Program berjalan di background (daemon)
e. Tidak boleh menggunakan fungsi system()
Contoh: ./program \* 34 7 /home/somi/test.sh
Program dengan argumen seperti contoh di atas akan menjalankan script test.sh setiap detik pada jam 07:34.

Kesulitan: kesulitan mengecek apakah bash sudah dijalankan dengan benar oleh program.

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
poin ini belum terselesaikan karena kami belum mendapatkan cara untuk membuat program c di dalam program c, yang kemudian bisa diexecute untuk membunuh program utama.

### 2.e Program utama mempunyai MODE_A dan MODE_B yang berguna dalam meng-kill program tersebut
- *MODE_A* akan membunuh program utama secara paksa, di mana semua pekerjaan yang sedang dilakukan akan diberhentikan pula.
- *MODE_B* akan membunuh program utama secara lebih halus, di mana pekerjaan mendownload dan menzip folder yang sudah (dan belum) berjalan akan tetap berjalan sampai folder dizip dan dihapus, namun tidak ada lagi folder yang akan terbuat.

poin ini belum terselesaikan karena kami belum mendapatkan cara membunuh dalam *MODE_B*, di mana seharusnya parent yang dibunuh dan loop while dalam daemon process akan berhenti. Yang masih terjadi adalah hanya pembuatan folder dan download di dalamnya yang berhenti, namun folder lain tetap dibuat.

sedangkan untuk *MODE_A*, yang kami dapatkan barulah code untuk menjalankan perintah `killall` terhadap program utama yang sudah di run. Namun, saat program ini dijalankan beserta dengan argumennya, perintah `killall` belum terlaksana.
~~~
int main(int argc, char const *argv[]) {
  if(argc == 2){
    if(strcmp(argv[1], "-a") == 0){
      char *argv[3] = {"killall", "soal2", NULL};
      execv("usr/bin/killall", argv);
    }
  }
  return 0;
}
~~~

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
