#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int write_to_file()
{
    int counter = 0;
    FILE *fp;
    if ((fp = fopen("data1.txt", "a+")) == NULL)
    {
        perror("fopen");
        return -1;
    }

    // Read the last number in the file
    while (fscanf(fp, "%d", &counter) != EOF);

    counter++;
    printf("after change : %d\n", counter);

    fprintf(fp, "\n%ld %d\n", (long int)getpid(), counter);
    fclose(fp);
}


volatile int ready_to_write  = 0; 
void handler_17(int n)
{
	printf("%d :someone is writting \n" , getpid());
}
void handler_chld(int n)
{
	printf("%d:someone write completly\n" , getpid());
	ready_to_write = 1 ; 
}
int main()
{
	// child write before parent write 
	// child signal over SIGCHLD when done to Parent 

	int pid;
	signal(17 , handler_17);
	signal(SIGCHLD , handler_chld);

	if (( pid = fork())  <0 )
	{
		perror("fork");
		return -1;
	}
	else if( pid == 0 )
	{
		//child
		printf("child : %d\n" , (int) getpid());
		kill( getppid() , 17);
		write_to_file();
	}
	else {
		// parent
		int n ;
		printf("parent : %d\n", (int) getpid());
		ready_to_write = 0;
		while(!ready_to_write)
               pause();  
		write_to_file();
	}
	return 0;
}

