/*#include <stdlib.h>
#include <stdio.h>

int main() {
    // Run the "ls -la" command
    int ret = system("ls -la");

    // Check if the command executed successfully
    if (ret == -1) {
        perror("system");
        return 1;
    }

    return 0;
}
*/
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>

void printFilePermissions(mode_t mode) {
    printf("%c", (S_ISDIR(mode)) ? 'd' : '-');
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

int main() {
    // Open the current directory
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    struct stat fileStat;

    // Read entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." entries
        if (entry->d_name[0] == '.') continue;

        // Get file status
        if (stat(entry->d_name, &fileStat) == -1) {
            perror("stat");
            continue;
        }

        // Print file details
        printFilePermissions(fileStat.st_mode); // File permissions
        printf(" %ld ", fileStat.st_nlink); // Number of links
        printf("%-8s ", getpwuid(fileStat.st_uid)->pw_name); // Owner
        printf("%-8s ", getgrgid(fileStat.st_gid)->gr_name); // Group
        printf("%8ld ", fileStat.st_size); // Size
        char timebuf[80];
        strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&fileStat.st_mtime));
        printf("%s ", timebuf); // Modification time
        printf("%s\n", entry->d_name); // File name
    }

    closedir(dir);
    return 0;
}