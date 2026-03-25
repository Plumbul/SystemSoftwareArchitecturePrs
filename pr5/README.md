# Практична робота 5: Про помилки роботи з пам’яттю в Linux та засоби їх усунення

![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

## Завдання. Зробити подвійне звільнення, яке інколи не падає через кешування малих блоків (tcache), але руйнує внутрішній стан allocator.

### Код програми
```с
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("--- Tcache Double Free Experiment ---\n");

    void *p1 = malloc(32);
    printf("Allocated block at: %p\n", p1);

    printf("First free(p1)...\n");
    free(p1);

    printf("Second free(p1) - Double Free!\n");
    free(p1); 

    printf("This line will not be reached if protection works.\n");
    return 0;
}
```
#### Результат запуску
![5_1](https://github.com/user-attachments/assets/48a23243-7997-4841-8a2b-a482a0539d44)
#### Висновок
Отриманий результат free(): double free detected in tcache 2 підтверджує функціонування захисних механізмів бібліотеки glibc версії 2.29 та новіших, що 
спрямовані на запобігання маніпуляціям із кешем потоків (tcache). При першому виклику free() алокатор записує у звільнений блок спеціальний 
маркер (per-thread key), який ідентифікує належність цього чанка до списку вільних блоків конкретного потоку. Під час другої спроби звільнення за тією ж 
адресою система звіряє цей ключ і, виявивши дублювання, негайно ініціює аварійне завершення процесу (SIGABRT). Такий підхід унеможливлює виникнення циклічних 
посилань у списку вільних блоків, що в іншому випадку дозволило б отримати ідентичні адреси при наступних викликах malloc() і призвело б до критичних вразливостей 
типу heap corruption.
