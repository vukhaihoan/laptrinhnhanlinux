#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void wakeup(int signum) {
    printf("Received signal to wake up.\n");
}

int main() {
    signal(SIGUSR1, wakeup);

    printf("Going to sleep indefinitely. My PID is %d.\n", getpid());
    pause();

    return 0;
}

