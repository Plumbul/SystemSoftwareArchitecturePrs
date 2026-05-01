#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *in, *out;
    char buf[256];

    /* 1. Використовуємо popen для читання виводу команди rwho */
    in = popen("rwho", "r");
    if (in == NULL) {
        perror("Error opening pipe for rwho");
        return EXIT_FAILURE;
    }

    out = popen("more", "w");
    if (out == NULL) {
        perror("Error opening pipe for more");
        pclose(in);
        return EXIT_FAILURE;
    }

    /* 3. Цикл читання з одного потоку та запису в інший */
    while (fgets(buf, sizeof(buf), in) != NULL) {
        fputs(buf, out);
    }

    /* 4. Закриття потоків */
    pclose(in);
    pclose(out);

    return EXIT_SUCCESS;
}