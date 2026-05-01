#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void explore_dir(const char *path) {
    char cmd[256];
    printf("\n=== Огляд каталогу: %s ===\n", path);
    sprintf(cmd, "ls -l %s | head -n 5", path);
    system(cmd);
}

void try_write(const char *filepath) {
    printf("Спроба запису у %s: ", filepath);
    int fd = open(filepath, O_WRONLY);
    if (fd == -1) {
        printf("ВІДМОВЛЕНО (%s)\n", strerror(errno));
    } else {
        printf("УСПІШНО\n");
        close(fd);
    }
}

int main() {
    explore_dir("~");
    explore_dir("/usr/bin");
    explore_dir("/etc"); 

    printf("\n--- Експерименти з обходом прав ---\n");

    try_write("/usr/bin/test_file");

    printf("Спроба читання /etc/shadow: ");
    FILE *f = fopen("/etc/shadow", "r");
    if (f == NULL) {
        printf("ВІДМОВЛЕНО (Permission denied)\n");
    } else {
        printf("УСПІШНО\n");
        fclose(f);
    }

    printf("Спроба виконання /usr/bin/whoami: ");
    if (access("/usr/bin/whoami", X_OK) == 0) {
        printf("УСПІШНО (є права на виконання)\n");
    } else {
        printf("ВІДМОВЛЕНО\n");
    }

    return 0;
}