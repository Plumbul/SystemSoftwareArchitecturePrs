#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int main() {
    DIR *d = opendir(".");
    struct dirent *e;
    while ((e = readdir(d))) {
        if (e->d_type == DT_REG) {
            printf("Delete %s? (y/n): ", e->d_name);
            char res;
            scanf(" %c", &res);
            if (res == 'y') unlink(e->d_name);
        }
    }
    closedir(d);
    return 0;
}