#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    DIR *d = opendir(".");
    struct dirent *e;
    while ((e = readdir(d))) {
        char *ext = strrchr(e->d_name, '.');
        if (ext && strcmp(ext, ".c") == 0) {
            printf("Grant read permission to others for %s? (y/n): ", e->d_name);
            char res;
            scanf(" %c", &res);
            if (res == 'y') {
                struct stat s;
                stat(e->d_name, &s);
                chmod(e->d_name, s.st_mode | S_IROTH);
            }
        }
    }
    closedir(d);
    return 0;
}