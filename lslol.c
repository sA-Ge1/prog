#include <stdlib.h>
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
