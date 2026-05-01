#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ucontext.h>
#include <unistd.h>

void posix_signal_handler(int sig, siginfo_t *siginfo, void *context) {
    ucontext_t *uctx = (ucontext_t *)context;
    
    printf("\n--- КРИТИЧНА ПОМИЛКА: Сигнал %d ---\n", sig);
    printf("Дамп регістрів:\n");

#if defined(__x86_64__)
    printf("RIP: 0x%llx\n", uctx->uc_mcontext.gregs[REG_RIP]);
    printf("RAX: 0x%llx  RBX: 0x%llx\n", uctx->uc_mcontext.gregs[REG_RAX], uctx->uc_mcontext.gregs[REG_RBX]);
    printf("RSP: 0x%llx  RBP: 0x%llx\n", uctx->uc_mcontext.gregs[REG_RSP], uctx->uc_mcontext.gregs[REG_RBP]);

#elif defined(__i386__)
    printf("EIP: 0x%x\n", uctx->uc_mcontext.gregs[REG_EIP]);
    printf("EAX: 0x%x  EBX: 0x%x\n", uctx->uc_mcontext.gregs[REG_EAX], uctx->uc_mcontext.gregs[REG_EBX]);

#elif defined(__aarch64__)
    printf("PC:  0x%llx\n", uctx->uc_mcontext.pc);
    printf("X0:  0x%llx  X1:  0x%llx\n", uctx->uc_mcontext.regs[0], uctx->uc_mcontext.regs[1]);
    printf("SP:  0x%llx\n", uctx->uc_mcontext.sp);

#else
    printf("Архітектура не підтримується для детального дампу.\n");
#endif

    exit(EXIT_FAILURE);
}

void setup_logging() {
    struct sigaction sa;
    sa.sa_sigaction = posix_signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGILL,  &sa, NULL);
    sigaction(SIGFPE,  &sa, NULL);
}

int main() {
    setup_logging();

    printf("Програма запущена. Виконую спробу падіння (Segmentation Fault)...\n");

    int *p = NULL;
    *p = 42; 

    return 0;
}