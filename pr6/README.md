# Практична робота 5: ІНСТРУМЕНТИ НАЛАГОДЖЕННЯ ДЛЯ ПРОБЛЕМ З ПАМ'ЯТТЮ

![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

## Завдання. Дослідити механізм copy-on-write, створивши програму з fork() та вимірявши кількість сторінкових переривань під час модифікації пам’яті за допомогою perf.

### Код програми
```с
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MEM_SIZE (100 * 1024 * 1024)

int main(int argc, char *argv[]) {
    char *buffer = malloc(MEM_SIZE);
    if (!buffer) return 1;
    memset(buffer, 'A', MEM_SIZE);

    printf("Parent PID: %d. Memory initialized.\n", getpid());

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        if (argc > 1 && strcmp(argv[1], "write") == 0) {
            printf("Child (PID: %d): Modifying memory (triggering CoW)...\n", getpid());
            for (int i = 0; i < MEM_SIZE; i += 4096) {
                buffer[i] = 'B';
            }
        } else {
            printf("Child (PID: %d): Only reading memory (CoW not triggered)...\n", getpid());
            char sum = 0;
            for (int i = 0; i < MEM_SIZE; i += 4096) {
                sum += buffer[i];
            }
            (void)sum; 
        }
        free(buffer);
        return 0;
    } else {
        wait(NULL);
        free(buffer);
        printf("Parent: Child finished.\n");
    }

    return 0;
}
```
#### Результат запуску
![6_1](https://github.com/user-attachments/assets/3e110086-1950-46f5-9642-d6effaf9f298)
#### Висновок
Результат виконання підтверджує ефективність стратегії "лінивого" копіювання в ядрі Linux. При відсутності операцій запису дочірній процес 
використовує фізичні сторінки пам'яті батьківського процесу в режимі спільного доступу. Це дозволяє уникнути надлишкового навантаження на підсистему пам'яті та 
мінімізує кількість сторінкових переривань (page faults), оскільки фізичне дублювання даних не ініціюється до моменту фактичної модифікації вмісту пам'яті.
