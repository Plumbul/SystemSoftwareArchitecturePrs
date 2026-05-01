#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void list(const char *name) {
    DIR *d = opendir(name);
    if (!d) return;
    struct dirent *e;
    while ((e = readdir(d))) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", name, e->d_name);
        printf("%s\n", path);
        struct stat s;
        stat(path, &s);
        if (S_ISDIR(s.st_mode)) list(path);
    }
    closedir(d);
}

int main() {
    list(".");
    return 0;
}