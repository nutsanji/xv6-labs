#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
	int pipe1[2], pipe2[2];
	char buffer[] = {'x'};//The transfort byte between two processes
	long length = sizeof(buffer);

	//Record the read and write file descriptors in the array p1, p2
	pipe(pipe1);
	pipe(pipe2);

	if(fork()==0){//In the child
		close(pipe1[1]); //Close the read end of pipe1
		close(pipe2[0]); //Close the write end of pipe2
		
		if(read(pipe1[0], buffer, length) != length){//the child read from pipe1
			printf("child: read error\n");
			exit(1);
		}
		
		printf("%d: received ping\n", getpid());

		if(write(pipe2[1], buffer, length) != length){//the child write to pipe2
			printf("child: wirte error\n");
			exit(1);
		}

		exit(0);
	}

	//In the parent
	close(pipe1[0]);
	close(pipe2[1]);

	if(write(pipe1[1], buffer, length) != length){//the parent write to pipe1
		printf("parent: write error\n");
		exit(1);
	}

	if(read(pipe2[0], buffer, length) != length){//the parent read from pipe2
		printf("parent: read error\n");
		exit(1);
	}

	printf("%d: received pong\n", getpid());
	wait(0);//wait the child exit
	exit(0);
}
	
/* The SEQUENCE really matter! */
