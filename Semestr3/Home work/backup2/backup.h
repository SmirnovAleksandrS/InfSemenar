#ifndef c_backup
#define c_backup

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int backup_copyfile(char* source_directory, char* destination_directory){
    struct stat* buffer = (struct stat*) malloc(sizeof(struct stat));
    // Выделение памяти на информацию о параметрах файла
    struct stat* buffer_gzip=(struct stat*) malloc(sizeof(struct stat));

    if (!fopen(source_directory, "r")){
        printf("Невозможно открыть файл\n");
        exit(1);
    }
    char* to_gz = (char*) calloc(1, 500);//память
    strcpy(to_gz, destination_directory);
    strcat(to_gz, ".gz");//склейка

    if (!fopen(to_gz, "r")){ //еще нет копии
        pid_t pid;
        pid = fork();
        if(pid == -1){
			printf("ERROR\n");
            exit(1);
        }
        if (pid == 0){
            printf("Make copy %s\n", destination_directory);
            execlp("cp", "cp", source_directory, destination_directory, NULL);
        }
        waitpid(pid, NULL, 0);
        pid = fork();
        if(pid == -1){
			printf("ERROR\n");
            exit(1);
        }
        if (pid == 0){
            execlp("gzip", "gzip", destination_directory, NULL);//
        }
        return 0;
    }
    stat(to_gz, buffer_gzip);              //информация об параметрах из to_gz пишется а buffer_gzip
    time_t gz_time = buffer_gzip->st_ctime;//время создания
    stat(source_directory, buffer);
    time_t fl_time = buffer->st_mtime;  //время последнего изменения
    free(buffer);               
    free(buffer_gzip);
    if(gz_time < fl_time){
        pid_t pid;
        pid = fork();
        if(pid == -1){
			printf("ERROR\n");
            exit(1);
        }
        if(pid == 0){
            printf("Modification of file %s \n", destination_directory);
            execlp("cp", "cp", source_directory, destination_directory, NULL);
        }
        waitpid(pid, NULL, 0);
        pid = fork();
        if(pid == -1){
			printf("ERROR\n");
            exit(1);
        }
        if (pid == 0){
            execlp("rm", "rm", to_gz, NULL);
        }
        waitpid(pid, NULL, 0);
        pid = fork();
        if(pid == -1){
			printf("ERROR\n");
            exit(1);
        }
        if (pid == 0){
            execlp("gzip", "gzip", destination_directory, NULL);
        }
        waitpid(pid, NULL, 0);
        return 0;
    }
    return 0;
}

int back_up(char* from_dir, char* to_dir){
    char* destination_directory;
    char* source_directory;
    DIR* dir;
    struct dirent* file;

    if((dir = opendir(from_dir)) == NULL){
        printf("Source directory doesnt exist\n");
        exit(1);
    }
    while((file = readdir(dir)) != NULL){
        source_directory = (char*) calloc(1, 500);
        destination_directory = (char*) calloc(1, 500);

        strcpy(source_directory, from_dir);
        strcat(source_directory, "/");
        strcat(source_directory, file->d_name);

        strcpy(destination_directory, to_dir);
        strcat(destination_directory, "/");
        strcat(destination_directory, file->d_name);

        struct stat* state_buffer = (struct stat*) malloc(sizeof(struct stat));
        stat(source_directory, state_buffer);
        if(strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0){
            if(S_ISDIR(state_buffer->st_mode)){
                if(opendir(destination_directory) == NULL){
                    pid_t pid;
                    pid = fork();
                    if(pid == -1){
			            printf("ERROR\n");
                        exit(1);
                    }
                    if(pid == 0){
                        execlp("mkdir", "mkdir", destination_directory, NULL);
                    }
                    waitpid(pid, NULL, 0);
                }
                back_up(source_directory, destination_directory);
            }
            if (S_ISREG(state_buffer->st_mode)){
                backup_copyfile(source_directory, destination_directory);
            }
        }
    }
    closedir(dir);
    return 0;
}

#endif