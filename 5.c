#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) return perror("open"), 1;

    struct flock lock = {F_WRLCK, SEEK_END, -100, 100, 0};
    printf("Press Enter to lock...\n"), getchar();
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        fcntl(fd, F_GETLK, &lock);
        return printf("Locked by PID: %d\n", lock.l_pid), close(fd), 1;
    }

    printf("Locked. Reading data...\n");
    lseek(fd, -50, SEEK_END);
    char buffer[101] = {0};
    read(fd, buffer, 100);
    printf("Data: %s\nPress Enter to unlock...\n", buffer), getchar();

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    printf("Unlocked.\n"), close(fd);
    return 0;
}
