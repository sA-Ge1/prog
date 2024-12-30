#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void ls_l(const char *path) 
{
    struct dirent *entry;
    struct stat fileStat;
    DIR *dir = opendir(path);
    
    if (!dir) {
        perror("Cannot open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_name[0] == '.') continue; 

        if (stat(entry->d_name, &fileStat) < 0) continue;

        char type;
        if (S_ISDIR(fileStat.st_mode))
            type = 'd';
        else if (S_ISREG(fileStat.st_mode))
            type = '-';
        else if (S_ISCHR(fileStat.st_mode))
            type = 'c';
        else if (S_ISBLK(fileStat.st_mode))
            type = 'b';
        else if (S_ISLNK(fileStat.st_mode))
            type = 'l';
        else if (S_ISFIFO(fileStat.st_mode))
            type = 'p';
        else if (S_ISSOCK(fileStat.st_mode))
            type = 's';
        else
            type = '?';

        char perms[10];
        perms[0] = (fileStat.st_mode & S_IRUSR) ? 'r' : '-';
        perms[1] = (fileStat.st_mode & S_IWUSR) ? 'w' : '-';
        perms[2] = (fileStat.st_mode & S_IXUSR) ? 'x' : '-';
        perms[3] = (fileStat.st_mode & S_IRGRP) ? 'r' : '-';
        perms[4] = (fileStat.st_mode & S_IWGRP) ? 'w' : '-';
        perms[5] = (fileStat.st_mode & S_IXGRP) ? 'x' : '-';
        perms[6] = (fileStat.st_mode & S_IROTH) ? 'r' : '-';
        perms[7] = (fileStat.st_mode & S_IWOTH) ? 'w' : '-';
        perms[8] = (fileStat.st_mode & S_IXOTH) ? 'x' : '-';
        perms[9] = '\0';

    
        char timeStr[13];
        strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", localtime(&fileStat.st_mtime));

        
        printf("%c%s %2ld %8ld %s %s\n",
               type, perms,
               fileStat.st_nlink,
               fileStat.st_size,
               timeStr,
               entry->d_name);
    }

    closedir(dir);
}

void cp(const char *src, const char *dest) 
{
    int srcFd = open(src, O_RDONLY);
    if (srcFd == -1) {
        perror("open src");
        return;
    }

    int destFd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (destFd == -1) 
    {
        perror("open dest");
        close(srcFd);
        return;
    }

    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = read(srcFd, buffer, sizeof(buffer))) > 0) 
    {
        if (write(destFd, buffer, bytesRead) != bytesRead) 
        {
            perror("write");
            break;
        }
    }

    if (bytesRead == -1)
        perror("read");

    close(srcFd);
    close(destFd);
}

void rm(const char *path) 
{
    if (unlink(path) == -1) 
    {
        perror("unlink");
    }
}

void mv(const char *src, const char *dest) 
{
    if (rename(src, dest) == -1) 
    {
        perror("rename");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <command> [arguments]\n", argv[0]);
        printf("Commands:\n");
        printf("  ls -l <directory>\n");
        printf("  cp <source> <destination>\n");
        printf("  rm <file>\n");
        printf("  mv <source> <destination>\n");
        return 1;
    }

    if (strcmp(argv[1], "ls") == 0 && argc == 4 && strcmp(argv[2], "-l") == 0) 
    {
        ls_l(argv[3]);
    } 
    else if (strcmp(argv[1], "cp") == 0 && argc == 4) 
    {
        cp(argv[2], argv[3]);
    } 
    else if (strcmp(argv[1], "rm") == 0 && argc == 3) 
    {
        rm(argv[2]);
    } 
    else if (strcmp(argv[1], "mv") == 0 && argc == 4) 
    {
        mv(argv[2], argv[3]);
    } 
    else 
    {
        printf("Invalid command or arguments.\n");
    }

    return 0;
}
