# Практична робота 3: Дослідження обмежень ресурсів у середовищі Docker
![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

## Завдання ЗАГАЛЬНЕ ДЛЯ ВСІХ
### Завдання 3.1
Запустіть Docker-контейнер і поекспериментуйте з максимальним лімітом ресурсів відкритих файлів. Для цього виконайте команди у вказаному порядку:
```с
$ ulimit -n
$ ulimit -aS | grep "open files"
$ ulimit -aH | grep "open files"
$ ulimit -n 3000
$ ulimit -aS | grep "open files"
$ ulimit -aH | grep "open files"
$ ulimit -n 3001
$ ulimit -n 2000
$ ulimit -n
$ ulimit -aS | grep "open files"
$ ulimit -aH | grep "open files"
$ ulimit -n 3000
```
#### Результат запуску
![3_1](https://github.com/user-attachments/assets/2cc2b610-cd83-48a0-8557-c8d218c483fd)

### Як наступне вправу, повторіть перераховані команди з root-правами.
#### Результат запуску

![3_2](https://github.com/user-attachments/assets/0e36c60c-de50-4a52-aeed-36c68f8f6b8e)

#### Висновок
Виконання команд ulimit без root-прав та з ними демонструє механізм обмеження системних ресурсів через розподіл на м’які та жорстк ліміти. М’який ліміт визначає поточне обмеження для процесів, тоді як жорсткий виступає верхньою межею, яку звичайний користувач не може перевищити. Експеримент підтверджує, що користувач має право самостійно знижувати обидва типи лімітів, проте після фіксації нового нижчого значення жорсткого ліміту спроба його підвищення заблокується системою з помилкою Operation not permitted. У середовищі Docker, навіть при роботі з root-правами всередині контейнера, діють аналогічні правила: користувач обмежений ресурсами, які виділив демон Docker при старті сесії. Таким чином, будь-яке ручне заниження ліміту в поточній сесії є незворотним без перезапуску оболонки або втручання адміністратора хостової системи, що підкреслює ієрархічну структуру керування доступом до файлових дескрипторів у Linux.
### Завдання 3.2
У Docker-контейнері встановіть утиліту perf(1). Поекспериментуйте з досягненням процесом встановленого ліміту.
#### Результат запуску

![3_3](https://github.com/user-attachments/assets/e339eee4-1f31-4d00-8e8e-8f54aec7c279)

#### Висновок
Спроба використання perf у Docker-контейнері продемонструвала залежність інструментарію від версії ядра хостової системи. Попри встановлення загального пакету linux-tools-generic, виникла невідповідність між версією інструментів у контейнері (6.8.0-106) та версією ядра хоста (6.8.0-101), що заблокувало роботу утиліти. Експеримент підтверджує, що для успішного профілювання в ізольованому середовищі версії linux-tools повинні суворо відповідати uname -r хоста. Встановлення низького ліміту ulimit -n 10 також спричинило помилки інтерпретатора ще до початку збору статистики, що свідчить про неможливість запуску складних діагностичних утиліт при критичному дефіциті файлових дескрипторів.
### Завдання 3.3
Напишіть програму, що імітує кидання шестигранного кубика. Імітуйте кидки, результати записуйте у файл, для якого попередньо встановлено обмеження на його максимальний розмір (max file size). Коректно обробіть ситуацію перевищення ліміту.
#### Код програми
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handle_sigxfsz(int sig) {
    const char *msg = "\nHard file size limit reached!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    _exit(EXIT_FAILURE); 
}

int main() {
    struct rlimit limit;
    const char *filename = "dice_results.txt";
    int roll;
    
    srand(time(NULL));

    signal(SIGXFSZ, handle_sigxfsz);

    limit.rlim_cur = 100;
    limit.rlim_max = 100;

    if (setrlimit(RLIMIT_FSIZE, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("File opening error");
        return 1;
    }

    printf("File size limit set to %ld bytes. Starting simulation...\n", limit.rlim_cur);

    while (1) {
        roll = (rand() % 6) + 1;
        
        if (fprintf(f, "Dice roll result: %d\n", roll) < 0) {
            perror("\nWrite error");
            break;
        }
        
        if (fflush(f) != 0) {
            perror("\nfflush error (limit exceeded)");
            break;
        }
        
        printf("Recorded roll: %d\n", roll);
        usleep(100000);
    }

    fclose(f);
    return 0;
}
```
#### Результат запуску
![photo_2026-03-20_17-25-44](https://github.com/user-attachments/assets/77401257-97b9-4547-989f-d0c238b4df58)
#### Висновок
Результати виконання програми підтверджують ефективність механізму setrlimit для контролю споживання дискового простору. Після встановлення обмеження RLIMIT_FSIZE на рівні 100 байт, ядро ОС дозволило процесу виконати лише обмежену кількість записів результатів кидків. Як тільки обсяг файлу досяг ліміту, операційна система надіслала процесу сигнал SIGXFSZ (File size limit exceeded). Програма продемонструвала коректну обробку критичної ситуації: перехопила сигнал через власний handler, вивела повідомлення про досягнення жорсткого ліміту та штатно завершила роботу. Це доводить можливість програмного обмеження росту файлів на рівні системних викликів для запобігання переповненню накопичувача.

### Завдання 3.4
Напишіть програму, що імітує лотерею, вибираючи 7 різних цілих чисел у діапазоні від 1 до 49 і ще 6 з 36. Встановіть обмеження на час ЦП (max CPU time) і генеруйте результати вибору чисел (7 із 49, 6 із 36). Обробіть ситуацію, коли ліміт ресурсу вичерпано.
#### Код програми
```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handle_sigxcpu(int sig) {
    const char *msg = "\n[SIGNAL] SIGXCPU: CPU time limit exceeded!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    _exit(EXIT_FAILURE);
}

void draw_numbers(int count, int max) {
    int numbers[count];
    for (int i = 0; i < count; i++) {
        int unique;
        do {
            unique = 1;
            numbers[i] = (rand() % max) + 1;
            for (int j = 0; j < i; j++) {
                if (numbers[i] == numbers[j]) {
                    unique = 0;
                    break;
                }
            }
        } while (!unique);
    }

    printf("Draw %d of %d: ", count, max);
    for (int i = 0; i < count; i++) printf("%d ", numbers[i]);
    printf("\n");
}

int main() {
    struct rlimit limit;
    srand(time(NULL));

    signal(SIGXCPU, handle_sigxcpu);

    limit.rlim_cur = 1; 
    limit.rlim_max = 2;

    if (setrlimit(RLIMIT_CPU, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    printf("CPU time limit set to %ld second. Starting lottery simulation...\n", limit.rlim_cur);

    unsigned long iteration = 0;
    while (1) {
        iteration++;
        if (iteration % 100000 == 0) {
            printf("Iteration %lu: ", iteration);
            draw_numbers(7, 49);
            draw_numbers(6, 36);
        }
    }

    return 0;
}
```

#### Результат запуску
![3_4](https://github.com/user-attachments/assets/6e9e7948-d46a-4ef8-a0e0-739f82cc043d)
#### Висновок
Виконання програми імітації лотереї підтверджує ефективність системного механізму обмеження процесорного часу RLIMIT_CPU на рівні ядра операційної системи. Програма встигла згенерувати понад 210 мільйонів комбінацій чисел, що свідчить про високу інтенсивність обчислень до моменту спрацювання ліміту. Як тільки сумарний час перебування процесу в стані виконання досяг встановленої межі в одну секунду, ядро надіслало процесу сигнал SIGXCPU. Завдяки попередньо зареєстрованому обробнику сигналу, програма не просто аварійно завершилася, а вивела діагностичне повідомлення про вичерпання ресурсу ЦП і штатно припинила роботу. Це демонструє можливість програмного контролю за тривалістю виконання завдань, що є критично важливим для запобігання монополізації обчислювальних потужностей окремими процесами або нескінченними циклами в багатокористувацьких системах.

### Завдання 3.5
Напишіть програму для копіювання одного іменованого файлу в інший. Імена файлів передаються у вигляді аргументів.
Програма має:
перевіряти, чи передано два аргументи, інакше виводити "Program need two arguments";
перевіряти доступність першого файлу для читання, інакше виводити "Cannot open file .... for reading";
перевіряти доступність другого файлу для запису, інакше виводити "Cannot open file .... for writing";
обробляти ситуацію перевищення обмеження на розмір файлу.
#### Код програми
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 4096

void handle_sigxfsz(int sig) {
    const char *msg = "\n[ERROR] SIGXFSZ: File size limit exceeded during copying!\n";
    write(STDOUT_FILENO, msg, strlen(msg));
    _exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int src_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Program need two arguments\n");
        return 1;
    }

    signal(SIGXFSZ, handle_sigxfsz);

    src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        fprintf(stderr, "Cannot open file %s for reading\n", argv[1]);
        return 1;
    }

    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        fprintf(stderr, "Cannot open file %s for writing\n", argv[2]);
        close(src_fd);
        return 1;
    }

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        
        if (bytes_written == -1) {
            if (errno == EFBIG) {
                fprintf(stderr, "\nError: File too large (limit reached)\n");
            } else {
                perror("\nWrite error");
            }
            break;
        }
    }

    if (bytes_read == -1) {
        perror("Read error");
    } else if (bytes_written != -1) {
        printf("File copied successfully from %s to %s\n", argv[1], argv[2]);
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}
```
#### Результат запуску
![3_5](https://github.com/user-attachments/assets/d71675dc-1f3b-4f02-8450-b073fc03cc96)

#### Висновок
Програма для копіювання файлів успішно реалізує алгоритм передачі даних між іменованими об’єктами файлової системи з урахуванням апаратних та програмних обмежень. Первинна перевірка кількості аргументів командного рядка запобігає некоректному виконанню при нестачі вхідних даних, що підтверджується виводом повідомлення Program need two arguments. Використання системних викликів для перевірки доступності дескрипторів на читання та запис дозволяє програмі ідентифікувати відсутність прав доступу або неіснуючі шляхи ще до початку ітераційного циклу. Впровадження обробника сигналу SIGXFSZ гарантує безпечне переривання операції при досягненні ліміту RLIMIT_FSIZE, встановленого користувачем або адміністратором через ulimit. Таким чином, програма не лише виконує свою основну функцію, але й демонструє механізми захисту від створення пошкоджених файлів великого обсягу, забезпечуючи цілісність системи квот та дискового простору.

### Завдання 3.6
Напишіть програму, що демонструє використання обмеження (max stack segment size). Підказка: рекурсивна програма активно використовує стек.
#### Код програми
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

void infinite_recursion(int depth) {
    char buffer[1024]; 
    if (depth % 100 == 0) {
        printf("Current stack depth: %d (approx %d KB used)\n", depth, depth);
    }
    infinite_recursion(depth + 1);
}

int main() {
    struct rlimit limit;

    limit.rlim_cur = 64 * 1024; 
    limit.rlim_max = 64 * 1024;

    if (setrlimit(RLIMIT_STACK, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    printf("Stack limit set to %ld bytes. Starting recursion...\n", limit.rlim_cur);

    infinite_recursion(1);

    return 0;
}

```
#### Результат запуску
![3_6](https://github.com/user-attachments/assets/e31d60f5-74ef-4087-8f11-890bebf49464)
#### Висновок
Результат запуску програми підтверджує критичну роль системних лімітів setrlimit у забезпеченні стабільності та безпеки операційної системи Linux. Встановлення обмежень на розмір файлу RLIMIT_FSIZE та процесорний час RLIMIT_CPU дозволило програмам коректно обробляти дефіцит ресурсів через механізми сигналів SIGXFSZ та SIGXCPU. Перевищення ліміту стека RLIMIT_STACK призводить до негайної зупинки процесу із помилкою сегментації для захисту пам'яті. Реалізація утиліти копіювання файлів довела практичну важливість перевірки прав доступу та обробки виняткових ситуацій на рівні системних викликів. Загалом, використання лімітів ресурсів дозволяє розробнику створювати відмовостійке програмне забезпечення, яке не монополізує апаратні потужності вузла. Такий підхід гарантує передбачувану поведінку прикладного коду навіть у випадках критичних помилок або нескінченних циклів.

## Завдання по ВАРІАНТАХ
### Реалізувати програму, що перевіряє максимальну кількість запущених процесів (ulimit -u).
#### Код програми
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main() {
    struct rlimit limit;
    int process_count = 0;

    limit.rlim_cur = 100; 
    limit.rlim_max = 200;

    if (setrlimit(RLIMIT_NPROC, &limit) != 0) {
        perror("setrlimit error");
        return 1;
    }

    printf("Max processes limit set to %ld. Starting fork bomb simulation...\n", limit.rlim_cur);

    while (1) {
        pid_t pid = fork();

        if (pid < 0) {
            if (errno == EAGAIN) {
                printf("\n[LIMIT REACHED] Cannot create more processes. Total created in this loop: %d\n", process_count);
            } else {
                perror("\nFork error");
            }
            break;
        } else if (pid == 0) {
            sleep(10);
            exit(0);
        } else {
            process_count++;
            if (process_count % 5 == 0) {
                printf("Processes created: %d\n", process_count);
            }
        }
    }

    printf("Cleaning up child processes...\n");
    return 0;
}
```
#### Результат запуску
![3_9](https://github.com/user-attachments/assets/0162882a-bea1-413f-b91c-0ffc466f99fa)

#### Висновок
Результат запуску цієї програми з лімітом RLIMIT_NPROC у 100 одиниць продемонстрував механіку обмеження кількості процесів на рівні ядра Linux. Програма успішно створила 32 дочірні процеси через системний виклик fork(), після чого виконання циклу було перервано через вичерпання квоти. Оскільки ліміт у 100 процесів є глобальним для користувача, сумарна кількість активних завдань у системі (включаючи оболонку Bash та саму програму) досягла встановленої межі саме на 33-й спробі розгалуження. Отримання помилки EAGAIN підтверджує, що операційна система веде суворий облік усіх потоків у реальному часі, блокуючи створення нових ідентифікаторів (PID) для захисту від перевантаження. Такий результат доводить ефективність використання setrlimit для запобігання неконтрольованому розмноженню процесів та забезпечення стабільності системних ресурсів.
