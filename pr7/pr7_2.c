#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main() {
    DIR *d;
    struct dirent *dir;
    struct stat s;
    d = opendir(".");
    while ((dir = readdir(d)) != NULL) {
        stat(dir->d_name, &s);
        printf((S_ISDIR(s.st_mode)) ? "d" : "-");
        printf((s.st_mode & S_IRUSR) ? "r" : "-");
        printf((s.st_mode & S_IWUSR) ? "w" : "-");
        printf((s.st_mode & S_IXUSR) ? "x" : "-");
        printf((s.st_mode & S_IRGRP) ? "r" : "-");
        printf((s.st_mode & S_IWGRP) ? "w" : "-");
        printf((s.st_mode & S_IXGRP) ? "x" : "-");
        printf((s.st_mode & S_IROTH) ? "r" : "-");
        printf((s.st_mode & S_IWOTH) ? "w" : "-");
        printf((s.st_mode & S_IXOTH) ? "x " : "- ");
        printf("%ld ", (long)s.st_nlink);
        printf("%s ", getpwuid(s.st_uid)->pw_name);
        printf("%s ", getgrgid(s.st_gid)->gr_name);
        printf("%lld ", (long long)s.st_size);
        char date[20];
        strftime(date, 20, "%b %d %H:%M", localtime(&s.st_mtime));
        printf("%s %s\n", date, dir->d_name);
    }
    closedir(d);
    return 0;
}