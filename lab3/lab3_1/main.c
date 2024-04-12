#include <stdio.h>
#include <signal.h>

void signal_handler(int signal) {
    printf("Hello, World!\n");
}

int main() {
    signal(SIGINT, signal_handler);
    while(1){}
    return 0;
}

