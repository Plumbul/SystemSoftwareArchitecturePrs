# Практична робота №10
![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04-E95420?style=for-the-badge&logo=ubuntu&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)

_____________
### Приклад 1
Crash handler із SA_SIGINFO і register dump
#### Код програми pr10_1.с
```c
#define _GNU_SOURCE
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <sys/ucontext.h>
#include <ucontext.h>
#include <unistd.h>
#include <stdlib.h>

static void wr_all(const char *s, unsigned long n) {
    while (n > 0) {
        ssize_t r = write(STDERR_FILENO, s, n);
        if (r <= 0) return;
        s += r;
        n -= (unsigned long)r;
    }
}

static void wr(const char *s) {
    unsigned long n = 0;
    while (s[n] != '\0') n++;
    wr_all(s, n);
}

static void wr_ch(char c) {
    wr_all(&c, 1);
}

static void wr_dec(long v) {
    char buf[32];
    int i = 0;
    unsigned long x;

    if (v < 0) {
        wr_ch('-');
        x = (unsigned long)(-(v + 1)) + 1UL;
    } else {
        x = (unsigned long)v;
    }

    do {
        buf[i++] = (char)('0' + (x % 10));
        x /= 10;
    } while (x != 0 && i < (int)sizeof(buf));

    while (i > 0) wr_ch(buf[--i]);
}

static void wr_hex(uint64_t v) {
    static const char hex[] = "0123456789abcdef";
    int started = 0;

    wr("0x");
    for (int shift = 60; shift >= 0; shift -= 4) {
        unsigned int nib = (unsigned int)((v >> shift) & 0xfU);
        if (nib != 0 || started || shift == 0) {
            wr_ch(hex[nib]);
            started = 1;
        }
    }
}

static void wr_ptr(const void *p) {
    wr_hex((uint64_t)(uintptr_t)p);
}

static void crash_handler(int sig, siginfo_t *si, void *ctx) {
    int saved_errno = errno;

    wr("\n=== crash captured ===\n");
    wr("signal: ");
    wr_dec(sig);
    wr("\n");

    if (si != NULL) {
        wr("si_code: ");
        wr_dec((long)si->si_code);
        wr("\n");

        wr("fault address: ");
        wr_ptr(si->si_addr);
        wr("\n");
    }

#if defined(__x86_64__)
    if (ctx != NULL) {
        ucontext_t *uc = (ucontext_t *)ctx;
        greg_t *g = uc->uc_mcontext.gregs;

        wr("RIP: "); wr_hex((uint64_t)g[REG_RIP]); wr("\n");
        wr("RSP: "); wr_hex((uint64_t)g[REG_RSP]); wr("\n");
        wr("RBP: "); wr_hex((uint64_t)g[REG_RBP]); wr("\n");
        wr("RAX: "); wr_hex((uint64_t)g[REG_RAX]); wr("\n");
        wr("RBX: "); wr_hex((uint64_t)g[REG_RBX]); wr("\n");
        wr("RCX: "); wr_hex((uint64_t)g[REG_RCX]); wr("\n");
        wr("RDX: "); wr_hex((uint64_t)g[REG_RDX]); wr("\n");
        wr("RSI: "); wr_hex((uint64_t)g[REG_RSI]); wr("\n");
        wr("RDI: "); wr_hex((uint64_t)g[REG_RDI]); wr("\n");
    }
#else
    wr("Register dump is implemented here only for x86-64.\n");
#endif

    errno = saved_errno;

    /*
     * Для production можна замість _exit() скинути handler на default
     * і повторно підняти сигнал, щоб отримати core dump.
     * Для навчального прикладу завершуємося простим exit status 128+sig.
     */
    _exit(128 + sig);
}

static void install_crash_handlers(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_sigaction = crash_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND;

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGBUS,  &sa, NULL);
    sigaction(SIGFPE,  &sa, NULL);
    sigaction(SIGILL,  &sa, NULL);
    sigaction(SIGABRT, &sa, NULL);
}

__attribute__((noinline))
static void crash_here(void) {
    volatile int *p = (int *)0;
    *p = 42;
}

int main(void) {
    install_crash_handlers();

    wr("About to crash. PID=");
    wr_dec((long)getpid());
    wr("\n");

    crash_here();
    return 0;
}
```
#### Команда компіляції та результати роботи 
<img width="952" height="378" alt="image" src="https://github.com/user-attachments/assets/dd4a8aab-53eb-4193-90c6-9ba757f05431" />

#### Висновок
Експеримент продемонстрував успішне перехоплення апаратного виключення та отримання низькорівневого контексту процесора. Це дозволяє точно діагностувати місце та причину критичної помилки (dereferencing null pointer) без використання зовнішніх дебагерів.

### Приклад 2
Correct sleeping: nanosleep() restart loop і clock_nanosleep() absolute deadline
#### Код програми pr10_2.с
```c
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static volatile sig_atomic_t got_usr1 = 0;

static void on_usr1(int sig) {
    (void)sig;
    got_usr1 = 1;
}

static int sleep_relative_ms(long ms) {
    struct timespec req = {
        .tv_sec = ms / 1000,
        .tv_nsec = (ms % 1000) * 1000000L
    };
    struct timespec rem;

    while (nanosleep(&req, &rem) == -1) {
        if (errno == EINTR) {
            req = rem;
            continue;
        }
        return -1;
    }

    return 0;
}

static void add_ms(struct timespec *t, long ms) {
    t->tv_sec += ms / 1000;
    t->tv_nsec += (ms % 1000) * 1000000L;

    while (t->tv_nsec >= 1000000000L) {
        t->tv_sec++;
        t->tv_nsec -= 1000000000L;
    }
}

static int sleep_periodic_absolute(struct timespec *deadline, long period_ms) {
    int rc;

    add_ms(deadline, period_ms);

    while ((rc = clock_nanosleep(CLOCK_MONOTONIC,
                                 TIMER_ABSTIME,
                                 deadline,
                                 NULL)) == EINTR) {
        /*
         * Reuse the same absolute deadline after a signal.
         * Це не накопичує drift так, як relative sleep loop.
         */
    }

    if (rc != 0) {
        errno = rc;
        return -1;
    }

    return 0;
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = on_usr1;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("PID=%ld. In another terminal: kill -USR1 %ld\n",
           (long)getpid(), (long)getpid());

    puts("Relative sleep for 5 seconds using nanosleep restart loop...");
    if (sleep_relative_ms(5000) == -1) {
        perror("nanosleep");
        return 1;
    }

    printf("Relative sleep finished. got_usr1=%d\n", got_usr1);

    puts("Now 5 periodic ticks with absolute clock_nanosleep deadlines...");

    struct timespec next;
    if (clock_gettime(CLOCK_MONOTONIC, &next) == -1) {
        perror("clock_gettime");
        return 1;
    }

    for (int i = 1; i <= 5; i++) {
        if (sleep_periodic_absolute(&next, 1000) == -1) {
            perror("clock_nanosleep");
            return 1;
        }
        printf("tick %d\n", i);
    }

    return 0;
}

```
#### Команда компіляції та результати роботи 
<img width="580" height="252" alt="image" src="https://github.com/user-attachments/assets/bca08363-6b8b-49d6-90fa-e8d5e78cf22b" />

#### Висновок
Програма наочно продемонструвала методи боротьби з часовим дрейфом та перериваннями системних викликів. Використання циклу перезапуску для nanosleep дозволяє гарантувати загальну тривалість паузи, проте лише метод абсолютних дедлайнів у clock_nanosleep забезпечує жорстку прив'язку до системного таймера CLOCK_MONOTONIC. Це виключає накопичення похибки (drift), яка зазвичай виникає через час виконання інструкцій самого процесу або обробку сигналів. Такий підхід є критично важливим для розробки систем реального часу та програм, що потребують прецизійної синхронізації періодичних подій. Виконання коду підтвердило, що архітектура таймерів у POSIX-сумісних системах дозволяє створювати відмовостійкі цикли, несприйнятливі до асинхронних зовнішніх сигналів.

### Приклад 3
Малий publisher-subscriber на real-time signals, sigqueue(), sigwaitinfo() і sigtimedwait()
#### Код програми pr10_3.с
```c
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

static long parse_long(const char *s, const char *what) {
    char *end = NULL;
    errno = 0;

    long v = strtol(s, &end, 10);

    if (errno != 0 || end == s || *end != '\0') {
        fprintf(stderr, "invalid %s: %s\n", what, s);
        exit(EXIT_FAILURE);
    }

    return v;
}

static int app_signal(void) {
    int sig = SIGRTMIN;

    if (sig > SIGRTMAX) {
        fprintf(stderr, "No available real-time signal\n");
        exit(EXIT_FAILURE);
    }

    return sig;
}

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s sub\n"
        "  %s sub-timeout\n"
        "  %s pub <subscriber-pid> <int> [<int> ...]\n"
        "Example:\n"
        "  terminal 1: %s sub\n"
        "  terminal 2: %s pub <pid> 10 20 30 -1\n",
        prog, prog, prog, prog, prog);
}

static void subscriber(int use_timeout) {
    int sig = app_signal();

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, sig);

    /*
     * Important:
     * Signal must be blocked before sigwaitinfo/sigtimedwait,
     * otherwise it can be delivered by default disposition or a handler.
     */
    if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
        die("sigprocmask");
    }

    printf("subscriber PID=%ld, waiting for signal %d (SIGRTMIN)\n",
           (long)getpid(), sig);
    fflush(stdout);

    for (;;) {
        siginfo_t si;
        memset(&si, 0, sizeof(si));

        int r;

        if (use_timeout) {
            struct timespec ts = {
                .tv_sec = 5,
                .tv_nsec = 0
            };
            r = sigtimedwait(&set, &si, &ts);
        } else {
            r = sigwaitinfo(&set, &si);
        }

        if (r == -1) {
            if (errno == EINTR) {
                continue;
            }

            if (use_timeout && errno == EAGAIN) {
                puts("timeout: no messages for 5 seconds");
                continue;
            }

            die(use_timeout ? "sigtimedwait" : "sigwaitinfo");
        }

        int value = si.si_value.sival_int;

        printf("received signal=%d value=%d from pid=%ld uid=%ld\n",
               r,
               value,
               (long)si.si_pid,
               (long)si.si_uid);
        fflush(stdout);

        if (value < 0) {
            puts("negative value received: shutting down subscriber");
            break;
        }
    }
}

static void publisher(pid_t pid, int argc, char **argv) {
    int sig = app_signal();

    for (int i = 3; i < argc; i++) {
        union sigval value;
        value.sival_int = (int)parse_long(argv[i], "message value");

        if (sigqueue(pid, sig, value) == -1) {
            die("sigqueue");
        }

        printf("sent value=%d to pid=%ld via signal=%d\n",
               value.sival_int,
               (long)pid,
               sig);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "sub") == 0) {
        subscriber(0);
    } else if (strcmp(argv[1], "sub-timeout") == 0) {
        subscriber(1);
    } else if (strcmp(argv[1], "pub") == 0) {
        if (argc < 4) {
            usage(argv[0]);
            return EXIT_FAILURE;
        }

        pid_t pid = (pid_t)parse_long(argv[2], "PID");
        publisher(pid, argc, argv);
    } else {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```
#### Команда компіляції та результати роботи 
<img width="629" height="176" alt="image" src="https://github.com/user-attachments/assets/080482af-0eb0-473b-b4b9-7207a6faef63" />
<img width="534" height="112" alt="image" src="https://github.com/user-attachments/assets/11eb2be3-5268-4532-a408-0cbb86a74140" />

#### Висновок
Проведене тестування підтвердило ефективність використання сигналів реального часу POSIX для організації асинхронного зв'язку між процесами. На відміну від стандартних сигналів, механізм sigqueue та sigwaitinfo дозволяє не лише переривати виконання процесу, а й передавати структуровану інформацію (ціле число, PID відправника, UID користувача). Експеримент показав, що сигнали реального часу автоматично стають у чергу на рівні ядра, що гарантує отримання всіх надісланих значень у тому порядку, в якому вони були відправлені. Блокування сигналу через sigprocmask дозволило підписнику коректно вилучати дані з черги без передчасної реакції системи за замовчуванням. Такий підхід є легковагою альтернативою складнішим методам IPC (наприклад, чергам повідомлень або сокетам) у задачах, де потрібна швидка передача коротких керуючих команд.

## Завдання за варіантами
Створіть файл, відкрийте його у програмі, виконайте fork() і зробіть запис у файл як у батьківському, так і в дочірньому процесі. Перевірте результат у вмісті файлу.
#### Код програми pr10_4.с
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    const char *filename = "fork_test.txt";
    
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Помилка відкриття файлу");
        return 1;
    }

    printf("Файл відкритий. Виконую fork()...\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Помилка fork");
        return 1;
    } else if (pid == 0) {
        const char *child_msg = "Це запис ДОЧІРНЬОГО процесу.\n";
        write(fd, child_msg, 50);
        printf("Дочірній процес виконав запис.\n");
        close(fd);
        exit(0);
    } else {
        const char *parent_msg = "Це запис БАТЬКІВСЬКОГО процесу.\n";
        write(fd, parent_msg, 50);
        printf("Батьківський процес виконав запис.\n");
        
        wait(NULL);
        close(fd);
    }

    printf("\nВміст файлу %s:\n", filename);
    system("cat fork_test.txt");

    return 0;
}
```
#### Команда компіляції та результати роботи 
<img width="756" height="164" alt="image" src="https://github.com/user-attachments/assets/a6ec7164-c06b-44fd-8e0c-4c90a78bf154" />

#### Висновок
Проведений експеримент підтвердив механізм копіювання контексту процесу під час виконання системного виклику fork(). Оскільки дочірній процес успадкував відкритий дескриптор файлу, обидва процеси продовжили використовувати спільний системний об’єкт опису файлу з єдиним покажчиком поточної позиції (offset). Це забезпечило коректне додавання даних до файлу без взаємного перекриття вмісту, попри паралельне виконання процесів. Результати аналізу вмісту файлу демонструють, що ядро Linux автоматично синхронізує доступ до файлового дескриптора між спорідненими процесами, що є базовим принципом при реалізації міжпроцесної взаємодії та перенаправлення потоків введення-виведення в операційних системах сімейства UNIX.
