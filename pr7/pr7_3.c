#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    FILE *f = fopen(argv[2], "r");
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, argv[1])) printf("%s", line);
    }
    fclose(f);
    return 0;
}