#include "pipex.h"

void print_status(enum e_status status)
{
	switch (status)
	{
	case INVALIDARG:
		printf("Invalid arguments\n");
		break;
	case NOFILE:
		printf("File doesn't exist\n");
		break;
	case EXECVE:
		printf("EXECVE ERROR\n");
		break;
	default:
		break;
	}
}


int main(int argc, char *argv[], char **env)
{
	// TODO file 인지 폴더인지 먼저
	if (argc != 5)
	{
		print_status(INVALIDARG);
		return (1);
	}
	char *filenameIn;
	char **commandIn;
	char **commandOut;
	char *filenameOut;

	filenameIn = argv[1];
	commandIn = ft_split(argv[2], ' ');
	commandOut = ft_split(argv[3], ' ');
	filenameOut = argv[4];

	int STDIN;
	int STDOUT;
	STDIN = dup(0);
	STDOUT = dup(1);


	int fdIn;

	if (access(filenameIn, R_OK) == -1)
	{
		print_status(NOFILE);
	}
	else
	{
		fdIn = open(filenameIn, O_RDONLY);
		dup2(fdIn, 0);
	}
	
	int fdOut;
	if (access(filenameOut, W_OK) == -1)
	{
		fdOut = open(filenameOut, O_CREAT, 0755);
	}
	else
	{
		fdOut = open(filenameOut, O_TRUNC, 0755);
	}

	pid_t pid;
	if (0 == (pid = fork())) // 자식 프로세스
	{
		execve("/usr/bin/grep", commandIn, env);
		// if (-1 == execve("/bin/ls", commandIn, env))
		{
			print_status(EXECVE);
			exit(1);
		}
	}
	else
	{
		waitpid(pid, NULL, 0); // 부모 프로세스
	}
	printf("-------------\n");
	lseek(fdIn, 0, SEEK_SET);
	// lseek(0, 0, SEEK_SET);

	// int pipex2[2];
	// pipe(pipex2);
	if (0 == (pid = fork())) // 자식 프로세스
	{
		if (-1 == execve("/usr/bin/grep", commandOut, env))
		// if (-1 == execve("/bin/ls", commandOut, env))
		{
			print_status(EXECVE);
			exit(1);
		}
	}
	else
	{
		waitpid(pid, NULL, 0); // 부모 프로세스
	}


	close(fdIn);
	close(fdOut);
	dup2(STDIN, 0);
	dup2(STDOUT, 1);
	printf("PIPEX END\n");
	return (0);
}