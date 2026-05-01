# Практична робота №7
![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

_____________
## Загальні завдання
1. Використайте popen(), щоб передати вивід команди rwho (команда UNIX) до more (команда UNIX) у програмі на C.
2. Напишіть програму мовою C, яка імітує команду ls -l в UNIX — виводить список усіх файлів у поточному каталозі та перелічує права доступу тощо.
3. Напишіть програму, яка друкує рядки з файлу, що містять слово, передане як аргумент програми.
4. Напишіть програму, яка виводить список файлів, заданих у вигляді аргументів, з зупинкою кожні 20 рядків, доки не буде натиснута клавіша
5. Напишіть програму, яка перелічує всі файли в поточному каталозі та всі файли в підкаталогах.
6. Напишіть програму, яка перелічує лише підкаталоги у алфавітному порядку.
7. Напишіть програму, яка показує користувачу всі його/її вихідні програми на C, а потім в інтерактивному режимі запитує,
   чи потрібно надати іншим дозвіл на читання (read permission); у разі ствердної відповіді — такий дозвіл повинен бути наданий.
8. Напишіть програму, яка надає користувачу можливість видалити будь-який або всі файли у поточному робочому каталозі. Має з’являтися ім’я файлу з запитом, чи слід його видалити.
9. Напишіть програму на C, яка вимірює час виконання фрагмента коду в мілісекундах.
10. Напишіть програму мовою C для створення послідовності випадкових чисел з плаваючою комою у діапазонах:
   (a) від 0.0 до 1.0
   (b) від 0.0 до n, де n — будь-яке дійсне число з плаваючою точкою.
   Початкове значення генератора випадкових чисел має бути встановлене так, щоб гарантувати унікальну послідовність.

### Завдання 1
Використайте popen(), щоб передати вивід команди rwho (команда UNIX) до more (команда UNIX) у програмі на C.

#### Код програми pr7_1.с
```c
#include <stdio.h>

int main() {
    FILE *in, *out;
    char buf[256];

    in = popen("rwho", "r");
    out = popen("more", "w");

    while (fgets(buf, sizeof(buf), in)) {
        fputs(buf, out);
    }

    pclose(in);
    pclose(out);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_1.c -o pr7_1
```
#### Результати роботи 

#### Висновок
Використання функції popen() дозволяє створювати програмні канали (pipes) для передачі потоку даних між різними системними утилітами UNIX безпосередньо через код на C.

### Завдання 2
Напишіть програму мовою C, яка імітує команду ls -l в UNIX — виводить список усіх файлів у поточному каталозі та перелічує права доступу тощо.

#### Код програми pr7_2.с
```c
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main() {
    DIR *d;
    struct dirent *dir;
    struct stat s;
    d = opendir(".");
    while ((dir = readdir(d)) != NULL) {
        stat(dir->d_name, &s);
        printf((S_ISDIR(s.st_mode)) ? "d" : "-");
        printf((s.st_mode & S_IRUSR) ? "r" : "-");
        printf((s.st_mode & S_IWUSR) ? "w" : "-");
        printf((s.st_mode & S_IXUSR) ? "x" : "-");
        printf((s.st_mode & S_IRGRP) ? "r" : "-");
        printf((s.st_mode & S_IWGRP) ? "w" : "-");
        printf((s.st_mode & S_IXGRP) ? "x" : "-");
        printf((s.st_mode & S_IROTH) ? "r" : "-");
        printf((s.st_mode & S_IWOTH) ? "w" : "-");
        printf((s.st_mode & S_IXOTH) ? "x " : "- ");
        printf("%ld ", (long)s.st_nlink);
        printf("%s ", getpwuid(s.st_uid)->pw_name);
        printf("%s ", getgrgid(s.st_gid)->gr_name);
        printf("%lld ", (long long)s.st_size);
        char date[20];
        strftime(date, 20, "%b %d %H:%M", localtime(&s.st_mtime));
        printf("%s %s\n", date, dir->d_name);
    }
    closedir(d);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_2.c -o pr7_2
```
#### Результати роботи 

#### Висновок
Робота з системними викликами stat та структурами dirent дає можливість отримувати детальні метадані файлової системи, такі як права доступу, власники та розмір файлів.

### Завдання 3
Напишіть програму, яка друкує рядки з файлу, що містять слово, передане як аргумент програми.

#### Код програми pr7_3.с
```c
#include <stdio.h>

int main() {
    FILE *in, *out;
    char buf[256];

    in = popen("rwho", "r");
    out = popen("more", "w");

    while (fgets(buf, sizeof(buf), in)) {
        fputs(buf, out);
    }

    pclose(in);
    pclose(out);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_3.c -o pr7_3
```
#### Результати роботи 

#### Висновок
Функція strstr() у поєднанні з потоковим читанням файлу дозволяє реалізувати ефективний алгоритм пошуку та фільтрації текстових даних за ключовим словом.

### Завдання 4
Напишіть програму, яка виводить список файлів, заданих у вигляді аргументів, з зупинкою кожні 20 рядків, доки не буде натиснута клавіша.

#### Код програми pr7_4.с
```c
#include <stdio.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        char line[256];
        int count = 0;
        while (fgets(line, sizeof(line), f)) {
            printf("%s", line);
            if (++count % 20 == 0) {
                printf("--Press Enter to continue--");
                getchar();
            }
        }
        fclose(f);
    }
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_4.c -o pr7_4
```
#### Результати роботи 

#### Висновок
Реалізація механізму призупинення виводу через getchar() дозволяє керувати великими обсягами текстової інформації, створюючи інтерфейс посторінкового перегляду.

### Завдання 5
Напишіть програму, яка перелічує всі файли в поточному каталозі та всі файли в підкаталогах.

#### Код програми pr7_5.с
```c
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list(const char *name) {
    DIR *d = opendir(name);
    if (!d) return;
    struct dirent *e;
    while ((e = readdir(d))) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", name, e->d_name);
        printf("%s\n", path);
        struct stat s;
        stat(path, &s);
        if (S_ISDIR(s.st_mode)) list(path);
    }
    closedir(d);
}

int main() {
    list(".");
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_5.c -o pr7_5
```
#### Результати роботи 

#### Висновок
Рекурсивний обхід каталогів є базовим методом для повної індексації ієрархічної структури файлової системи, включаючи всі вкладені рівні.

### Завдання 6
Напишіть програму, яка перелічує лише підкаталоги у алфавітному порядку.

#### Код програми pr7_6.с
```c
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main() {
    struct dirent **n;
    int count = scandir(".", &n, NULL, alphasort);
    for (int i = 0; i < count; i++) {
        if (n[i]->d_type == DT_DIR) {
            if (strcmp(n[i]->d_name, ".") != 0 && strcmp(n[i]->d_name, "..") != 0)
                printf("%s\n", n[i]->d_name);
        }
        free(n[i]);
    }
    free(n);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_2.c -o pr7_6
```
#### Результати роботи 

#### Висновок
Використання системної функції scandir() з параметром alphasort значно спрощує процес отримання відсортованого за алфавітом списку об'єктів файлової системи.

### Завдання 7
Напишіть програму, яка показує користувачу всі його/її вихідні програми на C, а потім в інтерактивному режимі запитує, чи потрібно надати іншим дозвіл на читання (read permission); у разі ствердної відповіді — такий дозвіл повинен бути наданий.

#### Код програми pr7_7.с
```c
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    DIR *d = opendir(".");
    struct dirent *e;
    while ((e = readdir(d))) {
        char *ext = strrchr(e->d_name, '.');
        if (ext && strcmp(ext, ".c") == 0) {
            printf("Grant read permission to others for %s? (y/n): ", e->d_name);
            char res;
            scanf(" %c", &res);
            if (res == 'y') {
                struct stat s;
                stat(e->d_name, &s);
                chmod(e->d_name, s.st_mode | S_IROTH);
            }
        }
    }
    closedir(d);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_7.c -o pr7_7
```
#### Результати роботи 

#### Висновок
Функція chmod() забезпечує програмне керування правами доступу, що дозволяє гнучко налаштовувати безпеку та рівні приватності файлів у багатокористувацьких системах.

### Завдання 8
Напишіть програму, яка надає користувачу можливість видалити будь-який або всі файли у поточному робочому каталозі. Має з’являтися ім’я файлу з запитом, чи слід його видалити.

#### Код програми pr7_8.с
```c
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int main() {
    DIR *d = opendir(".");
    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_type == DT_REG) {
            printf("Delete %s? (y/n): ", e->d_name);
            char res;
            scanf(" %c", &res);
            if (res == 'y') unlink(e->d_name);
        }
    }
    closedir(d);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_8.c -o pr7_8
```
#### Результати роботи 

#### Висновок
Системний виклик unlink() є основним інструментом для видалення посилань на файли, що призводить до їх остаточного видалення з диска після закриття всіх дескрипторів.

### Завдання 9
Напишіть програму на C, яка вимірює час виконання фрагмента коду в мілісекундах.

#### Код програми pr7_9.с
```c
#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (long i = 0; i < 100000000; i++);
    gettimeofday(&end, NULL);
    long ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Execution time: %ld ms\n", ms);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_9.c -o pr7_9
```
#### Результати роботи 

#### Висновок
Використання структури timeval та функції gettimeofday() дозволяє проводити точний моніторинг продуктивності коду з мікросекундною роздільною здатністю.

### Завдання 10
Напишіть програму мовою C для створення послідовності випадкових чисел з плаваючою комою у діапазонах:
   (a) від 0.0 до 1.0
   (b) від 0.0 до n, де n — будь-яке дійсне число з плаваючою точкою.
   Початкове значення генератора випадкових чисел має бути встановлене так, щоб гарантувати унікальну послідовність.

#### Код програми pr7_10.с
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    double n;
    printf("Enter n: ");
    scanf("%lf", &n);
    srand(time(NULL));
    double r1 = (double)rand() / RAND_MAX;
    double r2 = r1 * n;
    printf("Range 0.0 - 1.0: %f\n", r1);
    printf("Range 0.0 - %f: %f\n", n, r2);
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc -Wall pr7_10.c -o pr7_10
```
#### Результати роботи 

#### Висновок
Застосування функції srand() з ініціалізацією через time(NULL) гарантує отримання унікальних послідовностей випадкових чисел при кожному новому запуску програм.

## Завдання по варіантах:
### Напишіть програму, яка імітує роботу черги задач (job queue), з підтримкою паузи, скасування та відновлення, але без використання потоків або сигналів.

#### Код програми pr7_11.с
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_JOBS 5
#define TOTAL_STEPS 5

typedef enum { READY, RUNNING, PAUSED, CANCELLED, COMPLETED } JobStatus;

typedef struct {
    int id;
    int progress;
    JobStatus status;
} Job;

void print_status(Job *jobs) {
    printf("\rQueue Status: ");
    for (int i = 0; i < MAX_JOBS; i++) {
        char s;
        if (jobs[i].status == COMPLETED) s = 'V';
        else if (jobs[i].status == PAUSED) s = 'P';
        else if (jobs[i].status == CANCELLED) s = 'X';
        else if (jobs[i].status == RUNNING) s = 'R';
        else s = '.';
        printf("[%d:%c] ", jobs[i].id, s);
    }
    fflush(stdout);
}

int main() {
    Job jobs[MAX_JOBS];
    for (int i = 0; i < MAX_JOBS; i++) {
        jobs[i].id = i + 1;
        jobs[i].progress = 0;
        jobs[i].status = READY;
    }

    int active_jobs = MAX_JOBS;
    int cycle = 0;

    while (active_jobs > 0) {
        for (int i = 0; i < MAX_JOBS; i++) {
            if (jobs[i].status == READY) jobs[i].status = RUNNING;

            if (jobs[i].status == RUNNING) {
                jobs[i].progress++;
                if (jobs[i].progress >= TOTAL_STEPS) {
                    jobs[i].status = COMPLETED;
                    active_jobs--;
                }
            }
        }

        print_status(jobs);

        if (cycle == 1) {
            printf("\n[Event] Job 2 PAUSED.\n");
            jobs[1].status = PAUSED;
        }
        if (cycle == 2) {
            printf("\n[Event] Job 3 CANCELLED.\n");
            jobs[2].status = CANCELLED;
            active_jobs--;
        }
        if (cycle == 4) {
            printf("\n[Event] Job 2 RESUMED.\n");
            jobs[1].status = RUNNING;
        }

        cycle++;
        for(long long j = 0; j < 500000000; j++); 
    }

    printf("\n\nAll jobs processed. Execution finished.\n");
    return 0;
}
```
#### Команда компіляції та запуск програми
```c
gcc pr7_11.c -o pr7_11
```
#### Результати роботи 

#### Висновок
Програма реалізує чергу задач за допомогою циклу подій та скінченного автомату без використання потоків. Кожна задача має свій статус (виконання, пауза, скасування), що дозволяє гнучко керувати процесом в одному потоці. Такий підхід виключає стан перегонів (race conditions) і є ефективним для систем з обмеженими ресурсами, де важливо зберігати повний контроль над плануванням завдань без системних переривань.
