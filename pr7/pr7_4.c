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