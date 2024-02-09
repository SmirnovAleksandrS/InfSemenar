#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>


int copyfile(char* source, char* destination){
    struct stat* buffer = (struct stat*) malloc(sizeof(struct stat));
    // Выделение памяти на информацию о параметрах файла
    struct stat* buffer_gzip=(struct stat*) malloc(sizeof(struct stat));

    char* to_gz = (char*) calloc(1, 500);//память
    strcpy(to_gz, destination);
    strcat(to_gz, ".gz");//склейка

    stat(to_gz, buffer_gzip);              //информация об параметрах из to_gz пишется а buffer_gzip
    time_t gz_time = buffer_gzip->st_ctime;//время создания
    stat(source, buffer);
    time_t fl_time = buffer->st_mtime;  //время последнего изменения
    free(buffer);
    free(buffer_gzip);

    if (!fopen(to_gz, "r") || (gz_time < fl_time)){ //еще нет копии
        pid_t pid;
        pid = fork();
        if(pid == -1){
			printf("ERROR in fork\n");
            exit(1);
        }
        if (pid == 0){
            printf("Make copy %s\n", destination);
            execlp("cp", "\\cp", source, destination, "-u",  NULL);
        }
        waitpid(pid, NULL, 0);  //ждем когда скопируется
        pid = fork();
        if(pid == -1){
			printf("ERROR\n");
            exit(1);
        }
        if (pid == 0){
            execlp("gzip", "gzip", destination, NULL); //сжимаем файл
        }
        waitpid(pid, NULL, 0);  //ждем когда скопируется
        return 0;
    }

    return 0;
}

int back_up(char* from_dir, char* to_dir){
    char* destination_directory;
    char* source_directory;
    DIR* dir;
    struct dirent* file;

    dir = opendir(from_dir);
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
			            printf("ERROR in fork\n");
                        exit(1);
                    }
                    if(pid == 0){
                        execlp("mkdir", "mkdir", destination_directory, NULL);  //рекурсивное создание внутренних папок
                    }
                    waitpid(pid, NULL, 0);
                }
                back_up(source_directory, destination_directory);   //переход на следуйщий уровень вложенности
            }
            if (S_ISREG(state_buffer->st_mode)){
                copyfile(source_directory, destination_directory);
            }
        }
    }
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Error in arguments! Too few or too many arguments.\n");
        exit(-1);
    }
    FILE *folder_from = fopen(argv[1], "r");
    if (folder_from == NULL){
        printf("Can't open folder (from backup)!\n");
        exit(-1);
    }
    fclose(folder_from);
    FILE *folder_to = fopen(argv[2], "r");
    if (folder_to == NULL){
        printf("Can't open folder (to backup)!\n");
        exit(-1);
    }
    fclose(folder_from);
    printf("%s\n", argv[1]);
    printf("%s\n", argv[2]);

    back_up(argv[1], argv[2]);
    return 0;
}
