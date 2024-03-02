#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define READEND 0
#define WRITEEND 1

int
main(int argc, char *argv[]){
	int p1[2];
	int p2[2];
	int pid;
	char buf[1];

	pipe(p1);
	pipe(p2);

	pid = fork();

	if (pid < 0)
		exit(1);
	else if (pid == 0) {
	// child process
		close(p1[WRITEEND]);
		close(p2[READEND]);
		read(p1[READEND], buf, 1); // 为啥父进程已经close(p1[READEND])了 子进程还能读到数据---- 按照chatgpt的说法是，子进程会继承父进程的文件描述符，所以子进程也会有p1[READEND]的文件描述符。 即使父进程close了，子进程还是可以读到数据
		printf("%d: received ping\n", getpid());
		write(p2[WRITEEND], " ", 1);
		close(p1[READEND]);
		close(p2[WRITEEND]);
		exit(0);
	} else {
	// parent process
		close(p1[READEND]);
		close(p2[WRITEEND]);
		write(p1[WRITEEND], " ", 1);
		read(p2[READEND], buf, 1);
		printf("%d: received pong\n", getpid());
		close(p1[WRITEEND]);
		close(p2[READEND]);
		exit(0);
	}
}


// int
// main(int argc, char *argv[])
// {
//     if (fork()==0){
//         //  child process
//         // print proc number
//         printf("%d : received ping\n", getpid());
//     } else {
//         printf("%d : received pong\n", getpid());
//         wait(0);
//     }
//     exit(0);
// }


// 上面这种实现是很随机的，因为不知道哪个进程会先执行，所以我们需要用到管道来进行进程间通信
/**
1112  ::  rreececievievde dpi npgo
ng
*/