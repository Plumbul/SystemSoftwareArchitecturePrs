#include <stdio.h>
#include <time.h>
#include <stdint.h>

int main() {
    time_t t;
    int bits = sizeof(t) * 8;
    
    printf("Architecture: %d-bit\n", bits);

    long long max_seconds = (1LL << (bits - 1)) - 1;
    printf("Mathematical max seconds: %lld\n", max_seconds);

    time_t test_time = (time_t)max_seconds;
    struct tm *time_info = gmtime(&test_time);

    while (time_info == NULL && test_time > 0) {
        test_time >>= 1; 
        time_info = gmtime(&test_time);
    }

    if (time_info != NULL) {
        printf("Last processable date by your system (UTC): %s", asctime(time_info));
        printf("Corresponding seconds: %lld\n", (long long)test_time);
    } else {
        printf("Error: Could not convert any positive time value.\n");
    }

    return 0;
}