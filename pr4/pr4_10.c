#include <stdio.h>
#include <stdlib.h>

void test_reuse(size_t size, const char* label) {
    printf("--- Testing %s (size: %zu bytes) ---\n", label, size);
    int reuse_count = 0;
    int iterations = 10;

    for (int i = 0; i < iterations; i++) {
        void *p1 = malloc(size);
        void *p2 = NULL;

        free(p1);
        p2 = malloc(size);

        if (p1 == p2) {
            reuse_count++;
            printf("  Iter %d: REUSED (%p)\n", i, p2);
        } else {
            printf("  Iter %d: NEW    (p1:%p, p2:%p)\n", i, p1, p2);
        }
        free(p2);
    }
    printf("Result: %d/%d reuses\n\n", reuse_count, iterations);
}

int main() {
    test_reuse(32, "Small block");
    
    test_reuse(1024, "Medium block");

    printf("--- Testing mismatched sizes ---\n");
    void *p1 = malloc(64);
    free(p1);
    void *p2 = malloc(1024);
    if (p1 == p2) printf("Mismatched REUSED\n");
    else printf("Mismatched NEW (Expected)\n");
    free(p2);

    return 0;
}