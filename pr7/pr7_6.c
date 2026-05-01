#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main() {
    struct dirent **n;
    int count = scandir(".", &n, NULL, alphasort);
    for (int i = 0; i < count; i++) {
        if (n[i]->d_type == DT_DIR) {
            if (strcmp(n[i]->d_name, ".") != 0 && strcmp(n[i]->d_name, "..") != 0)
                printf("%s\n", n[i]->d_name);
        }
        free(n[i]);
    }
    free(n);
    return 0;
}