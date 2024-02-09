#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define MAX_STROKE_NUMBER 100

struct process{
    int delay;
    char name[100];
};

int comp_struct(const void * elem1, const void * elem2){
    struct process el1 = *((struct process*)elem1);
    struct process s = *((struct process*)elem2);
    
    return el1.delay - s.delay;
}

int main(){
    // FILE fd;
    FILE *fd = fopen("test.txt", "r");
    if (fd == NULL){
        printf("Can't open file");
    } else {
        char str[BUFFER_SIZE];
        // fgets(&str, BUFFER_SIZE, fd);
        struct process prcs[MAX_STROKE_NUMBER];
        int i = 0, len = 0;
        while (fscanf(fd, "%d%s", &(prcs[i].delay), prcs[i].name) != EOF){
            i++;
        }
        fclose(fd);
        len = i - 1;
        printf("Readed data\n");
        for (i = len; i>=0; i--){
            printf("%d %s\n",prcs[i].delay, prcs[i].name);
        }
        //try to sort
        printf("Sorted data %d\n", len);
        qsort(prcs, len+1, sizeof(*prcs), comp_struct);
        for (i = 0; i <= len; i++){                                                         
            printf("%d %s\n",prcs[i].delay, prcs[i].name);
        }
        printf("Start create proceses\n");
        int last_time = 0;
        for (i = 0; i <= len; i ++){
            sleep(prcs[i].delay - last_time);
            pid_t pid;
            pid = fork();
            if (pid == -1){
                printf("Error");
            } else if (pid == 0) {
                (void) execle(prcs[i].name, ".");
                printf("Error");
                exit(-1);
                return 0;
            }
        }
    }
}
