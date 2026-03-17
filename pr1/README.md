# Практична робота №1
![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

_____________
## Загальне завдання
  1. Встановити Ubuntu (dual-boot, WSL або VirtualBox).
  2. Запустити операційну систему.
  3. Встановити необхідні інструменти для програмування.
  4. Створити, скомпілювати та виконати просту програму мовою C.

Під час виконання практичної роботи було встановлено та запущено Ubuntu на базі VirtualBox. 
Потім була створена проста програма мовою С - hello.c
### Код програми
```c
#include <stdio.h>

int main() {
    printf("Hello Ubuntu\n");
    return 0;
}
```
### Компіляція та запуск програми
```c
gcc hello.c -o hello
./hello
```
### Результат виконання програми
```c
Hello Ubuntu
```
### Висновок
Програма успішно виводить текст, який було заплановано виводити.

## Завдання 1.4
У вас є програма на C, яка складається з кількох модулів (main.c, module1.c, module2.c) та використовує бібліотеку pthread.
  - Напишіть команду компіляції з підтримкою багатопоточності (-pthread).
  - Змініть код так, щоб він використовував OpenMP (#pragma omp parallel).
  - Використовуйте valgrind або gprof для аналізу продуктивності.
  - gcc -Wall -Wextra -pthread main.c module1.c module2.c -o threaded_program
  - Оптимізуйте код для роботи з багатоядерними процесорами.
  - Використовуйте асинхронний ввід/вивід (aio.h) замість стандартного stdio.h.
  - Реалізуйте версію програми на C++ з використанням std::thread.

### Підготовка файлів для виконання завдання
  Враховуючи великий обсяг коду, який необхідно написати, було прийнято рішення встановити зручний текстовий редактор коду - micro. Також оскільки для виконання завдання необхідно декілька файлів, на початку було прийнято рішення написати хеддерний файл modules.h

#### Код файлу modules.h
```c
#ifndef MODULES_H
#define MODULES_H

void* add_func(void* arg);
void* sub_func(void* arg);

#endif
```
Далі створено програму main.с яка містить операції з потоками - їх створення та паралельне виконання операційщодо змінної initial_data. Функції, які оперують цією змінною містяться у програмах module1.с та module2.с - у них створено два цикли, один з яких додає до заданого числа одиницю мільйон разів, а другий одиницю мільон разів віднімає. Програми мають на меті продемонструвати що великі за обсягом паралельні операції будуть призводити до непередбачуваних результатів за відсутності синхронізації процесів. 
#### Код програми main.c
```c
#include <stdio.h>
#include <pthread.h>
#include "modules.h"

int main() {
    pthread_t thread1, thread2;
    int initial_data = 100;

    printf("Initial value: %d\n", initial_data);

    pthread_create(&thread1, NULL, add_func, &initial_data);
    pthread_create(&thread2, NULL, sub_func, &initial_data);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Result: %d\n", initial_data);
    return 0;
}
```
#### Код програми module1.с
```c
#include <stdio.h>
#include "modules.h"

void* add_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) *val +=1;
    return NULL;
}
```
#### Код програми module2.с
```c
#include <stdio.h>
#include "modules.h"

void* sub_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) *val-=1;
    return NULL;
}
```
### Напишіть команду компіляції з підтримкою багатопоточності (-pthread).

#### Компіляція та запуск програм
  ```c
gcc main.c module1.c module2.c -o pr1 -pthread
./pr1
```
### Результат виконання програми

![photo_2026-03-02_15-49-47](https://github.com/user-attachments/assets/8662b6bc-44e2-4b45-b8af-2c88899543e7)

#### Висновок
Результат виконання програми демонструє що великі за обсягом паралельні операції при відсутності синхронізації неминуче призводять до спотворення даних через паралельне опрацювання потоків сучасними операційними системами.

### Змініть код так, щоб він використовував OpenMP (#pragma omp parallel).

Написана програма використовувала виклики pthread_create та pthread_join, що вимагало ручного керування кожним потоком. Для виконання наступного завдання код було модифіковано наступним чином:
- Використано бібліотеку <omp.h>, яка надає інтерфейс для моделі паралельного програмування OpenMP.
- Використано директиву #pragma omp parallel sections, яка замість явного створення структур для потоків використовує механізм секцій, тобто вказує компілятору створити групу потоків, де кожна вкладена секція (#pragma omp section) буде виконуватися окремим потоком паралельно.

#### Код програми main.c
```c
#include <stdio.h>
#include <omp.h>
#include "modules.h"

int main() {
    int initial_data = 100;

    printf("Initial value: %d\n", initial_data);

   #pragma omp parallel sections
    {
        #pragma omp section
        {
            add_func(&initial_data);
        }

        #pragma omp section
        {
            sub_func(&initial_data);
        }
    }

    printf("Result (OpenMP): %d\n", initial_data);
    return 0;
}
```

#### Компіляція та запуск програми
  ```c
gcc -forenmp main.c module1.c module2.c -o pr1_omp
./pr1_omp
```
Використано прапорець компіляції -forenmp, який активує обробку директив #pragma.
#### Результат виконання програми

![photo_2026-02-17_18-07-32](https://github.com/user-attachments/assets/4b8fb3db-954e-40c5-a42f-e6582f545f3a)

#### Висновок
Зміна спосібу створення потоків на OpenMP вирішила проблему одночасного доступу до пам'яті. і тепер потоки не конфліктують при спробі одночасно змінити initial_data.

### Використовуйте valgrind або gprof для аналізу продуктивності.

![photo_2026-03-02_15-53-17](https://github.com/user-attachments/assets/7b836fec-5116-4164-b78e-c4fb6fab1fcc)
![photo_2026-02-17_19-05-36](https://github.com/user-attachments/assets/231dccff-9d65-4e57-ace3-8b8d917d67cb)

#### Висновок
Аналіз через valgrind не виявив помилки у програмі, яка проводить операції через паралельну обробку. Аналогічний результат був і при аналізі початковій програми, що може мати як причину малу кількість операцій для аналізу через що програма завершується надто швидко, однак збільшення операцій з 1 000 000 до 100 000 000 не призвело до зміни результатів.

### gcc -Wall -Wextra -pthread main.c module1.c module2.c -o threaded_program

![9](https://github.com/user-attachments/assets/d504d1fd-e534-43a5-a32d-1665123213f3)

#### Висновок 
Виконання команди компіляції, яка містить прапорці:
- -Wall, який вмикає більшість попереджень компілятора.
- -Wextra, який вмикає додаткові попередження, які не охоплюються прапорцем -Wall.
має такий самий результат як і без них, оскільки помилок в коді програм нема.

### Оптимізуйте код для роботи з багатоядерними процесорами
У програмах використано директиву #pragma omp parallel for reduction(+:local_sum). Кожен потік тепер рахує свою частину суми локально, а в кінці результат додається до загальної змінної через #pragma omp atomic.
#### Код програми main.c
```c
#include <stdio.h>
#include "modules.h"

int main() {
    int initial_data = 100;
    printf("Initial value: %d\n", initial_data);

    add_func(&initial_data);
    sub_func(&initial_data);

    printf("Result: %d\n", initial_data);
    return 0;
}
```
#### Код програми module1.с
```c
#include <omp.h>
#include "modules.h"

void add_func(int *val) {
    int local_sum = 0;

    #pragma omp parallel for reduction(+:local_sum)
    for (int i = 0; i < 1000000; i++) {
        local_sum++;
    }

    #pragma omp atomic
    *val += local_sum;
}
```
#### Код програми module2.с
```c
#include "modules.h"
#include <omp.h>

void sub_func(int *val) {
    int local_sub = 0;

    #pragma omp parallel for reduction(+:local_diff)
    for (int i = 0; i < 1000000; i++) {
        local_sub++; 
    }

    #pragma omp atomic
    *val -= local_sub;
}
```
#### Результати роботи 
![photo_2026-03-17_16-06-25](https://github.com/user-attachments/assets/a928e963-d1b7-45fe-a4b0-bc4228f29e43)
#### Висновок
Такий підхід усуває конфлікти доступу і дозволяє програмі масштабуватися на будь-яку кількість ядер без втрати точності, що збільшує швидкість обробки великих масивів даних.

### Використовуйте асинхронний ввід/вивід (aio.h) замість стандартного stdio.h.
Для створення асинхронного виводу додано спеціалізовану бібліотеку #include <aio.h>
#### Код файлу modules.h
```c
#ifndef MODULES_H
#define MODULES_H

void* add_func(void* arg);
void* sub_func(void* arg);

#endif
```

#### Код програми main.c
```c
#include <stdio.h>
#include <string.h>
#include <aio.h>
#include <unistd.h>
#include <pthread.h>
#include "modules.h"

void async_print(const char *msg) {
    static struct aiocb cb;
    memset(&cb, 0, sizeof(struct aiocb));
    cb.aio_fildes = STDOUT_FILENO;
    cb.aio_buf = (void *)msg;
    cb.aio_nbytes = strlen(msg);

    if (aio_write(&cb) == -1) {
        perror("aio_write failed");
    }
}

int main() {
    int initial_data = 100;
    char msg[64];
    sprintf(msg, "Initial value: %d\n", initial_data);
    async_print(msg);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, add_func, &initial_data);
    pthread_create(&t2, NULL, sub_func, &initial_data);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Result: %d\n", initial_data);
    return 0;
}
```
#### Код програми module1.с
```c
#include "modules.h"
#include <stdio.h>

void* add_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) {
        *val += 1;
    }
    return NULL;
}
```
#### Код програми module2.с
```c
#include "modules.h"
#include <stdio.h>

void* sub_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) {
        *val *= 1;
    }
    return NULL;
}
```

#### Компіляція та запуск програми

![3](https://github.com/user-attachments/assets/e631d97f-33ba-45cf-89dd-5bd8014a1459)

#### Висновок
Асинхронний вивід дозволяє ядрам виконувати обчислення, поки ОС займається повільними операціями виводу в термінал або файл. Однак це всеодно не усуває проблему рандомності виведених даних оскільки потоки не синхронізовані.

### Реалізуйте версію програми на C++ з використанням std::thread.
Початковий код перероблено мовою С++
#### Код файлу modules.hpp
```c
#ifndef MODULES_HPP
#define MODULES_HPP

void add_func(int *val);
void sub_func(int *val);

#endif

#endif
```

### Код програми main.cpp
```c
#include <iostream>
#include <thread>
#include "modules.hpp"

int main() {
    int initial_data = 100;

    std::cout << "Initial value: " << initial_data << std::endl;

    std::thread thread1(add_func, &initial_data);
    std::thread thread2(sub_func, &initial_data);

    thread1.join();
    thread2.join();

    std::cout << "Result: " << initial_data << std::endl;

    return 0;
}
```
#### Код програми module1.сpp
```c
#include "modules.hpp"

void add_func(int *val) {
    for (int i = 0; i < 1000000; i++) {
        *val += 1;
    }
}
```
#### Код програми module2.сpp
```c
#include "modules.hpp"

void sub_func(int *val) {
    for (int i = 0; i < 1000000; i++) {
        *val -= 1;
    }
}
```

#### Компіляція та запуск програми

![5](https://github.com/user-attachments/assets/d3e32692-f508-4404-ba7a-ad69355a94f2)

#### Висновок
Результат програми на с++ так само рандомний через відсутність синхорнізації потоків. 
