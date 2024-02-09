#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int numChildren = 5;
    pid_t childPids[5];

    for (int i = 0; i < numChildren; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Код, выполняемый дочерним процессом
            printf("Child process %d\n", i);
            sleep(3);
            exit(EXIT_SUCCESS);
        } else {
            childPids[i] = pid;
        }
    }

    int activeChildren = numChildren;

    while (activeChildren > 0) {
        // Неблокирующий вызов waitpid с использованием fcntl
        for (int i = 0; i < numChildren; i++) {
            int status;
            pid_t result = waitpid(childPids[i], &status, WNOHANG);

            if (result == -1) {
                perror("waitpid");
                exit(EXIT_FAILURE);
            } else if (result > 0) {
                // Дочерний процесс завершился
                printf("Child process with PID %d has terminated.\n", result);
                activeChildren--;
            }
            // Если result == 0, то дочерний процесс еще не завершился
        }

        // Дополнительная логика родительского процесса может быть добавлена здесь

        sleep(1); // Небольшая задержка, чтобы избежать бесконечного цикла
    }

    printf("All child processes have terminated.\n");

    return 0;
}
