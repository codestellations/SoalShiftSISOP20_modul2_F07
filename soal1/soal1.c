//
//  main.c
//  modul2_soal1
//
//  Created by Liizza Sulistio on 11/03/20.
//  Copyright © 2020 Liizza Sulistio. All rights reserved.
//

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>

void invalidArg() /*digunakan untuk memberi tahu user jika input yang dimasukkan tidak sesuai*/
{
    printf("Invalid Input\n");
}

/*prototype function*/
int checkNum(char arg[]);
int checkAst(char arg[]);

int main(int argc, char **argv)
{
    if(argc != 5) /*karena argumen yg dibutuhkan 4*/
    {
        invalidArg();
        printf("Only 4 arguments needed, consists of: seconds | minutes | hour | pathfile\n");
        exit(EXIT_FAILURE);
    }
    int arguments[4];
    
   int hr = 0, min = 0, sec = 0;
   int curr_hr, curr_min, curr_sec, i;
   
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
    /*mengecek ketentuan input apakah sudah sesuai atau belum*/
    if(sec < -1 || sec > 59 || min < -1 || min > 59 || hr < -1 || hr > 23)
       {
           invalidArg();
           printf("Input format: seconds (0 - 59) | minutes (0 - 59) | hour (0 - 23)\n");
           exit(EXIT_FAILURE);
       }
    
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
}

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
