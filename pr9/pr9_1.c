#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *fp = popen("getent passwd", "r");
    if (!fp) return 1;

    char line[256], user[50];
    int uid;
    uid_t my_uid = getuid();

    printf("Інші звичайні користувачі:\n");
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^:]:%*[^:]:%d", user, &uid) == 2) {
            if (uid >= 1000 && uid < 65534 && uid != (int)my_uid) {
                printf("- %s (UID: %d)\n", user, uid);
            }
        }
    }

    pclose(fp);
    return 0;
}