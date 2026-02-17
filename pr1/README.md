# Практична робота №1
![pylounge](https://img.shields.io/ubuntu/v/24.4)
![pylounge](https://img.shields.io/badge/C++-%2300599C.svg?&style=for-the-badge&logo=c%2B%2B&logoColor=white)

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

### Напишіть команду компіляції з підтримкою багатопоточності (-pthread).
  Враховуючи великий обсяг коду, який необхідно написати, було прийнято рішення встановити зручніший текстовий редактор коду - micro. Також оскільки для виконання завдання необхідно декілька файлів, на початку було прийнято рішення написати хеддерний файл modules.h

### Код файлу modules.h
```c
#ifndef MODULES_H
#define MODULES_H

void* add_func(void* arg);
void* sub_func(void* arg);

#endif
```
Далі про створено програму main.с яка містить операції з потоками - їх створення та паралельне виконання операційщодо змінної initial_data. Функції, які оперують цією змінною містяться у програмах module1.с та module2.с - у них створено два цикли, один з яких додає до заданого числа одиницю мільйон разів, а другий одиницю мільон разів віднімає. Програми мають на меті продемонструвати що великі за обсягом паралельні операції будуть призводити до непередбачуваних результатів за відсутності синхронізації процесів. 
### Код програми main.c
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
### Код програми module1.с
```c
#include <stdio.h>
#include "modules.h"

void* add_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) *val +=1;
    return NULL;
}
```
### Код програми module2.с
```c
#include <stdio.h>
#include "modules.h"

void* sub_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) *val-=1;
    return NULL;
}
```

### Компіляція та запуск програми
  ```c
gcc main.c module1.c module2.c -o pr1 -pthread
./pr1
```
### Результат виконання програми

Картінка

### Висновок
Результат виконання програми демонструє що великі за обсягом паралельні операції при відсутності синхронізації неминуче призводять до спотворення даних через паралельне опрацювання потоків сучасними операційними системами.

 ### Змініть код так, щоб він використовував OpenMP (#pragma omp parallel).
 
 ### Код програми main.c
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

### Компіляція та запуск програми
  ```c
gcc -forenmp main.c module1.c module2.c -o pr1_omp
./pr1_omp
```

### Результат виконання програми

Картінка

### Висновок

### Використовуйте valgrind або gprof для аналізу продуктивності.
Буде проведено аналіз 
Картінка
### Висновок
### gcc -Wall -Wextra -pthread main.c module1.c module2.c -o threaded_program
Картінка
### Оптимізуйте код для роботи з багатоядерними процесорами
Змінено код 
### Код програми main.c
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
### Код програми module1.с
```c
#include <stdio.h>
#include "modules.h"

void* add_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) *val +=1;
    return NULL;
}
```
### Код програми module2.с
```c
#include <stdio.h>
#include "modules.h"

void* sub_func(void* arg) {
    int *val = (int*)arg;
    for (int i = 0; i < 1000000; i++) *val-=1;
    return NULL;
}
```





### Використовуйте асинхронний ввід/вивід (aio.h) замість стандартного stdio.h.

### Реалізуйте версію програми на C++ з використанням std::thread.

#### Версія з

