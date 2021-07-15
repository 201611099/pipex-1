#include "pipex.h"

void print_status(enum e_status status)
{
	switch (status)
	{
	case INVALIDARG:
		perror("Invalid arguments");
		break;
	case NOFILE:
		perror("File doesn't exist");
		break;
	case EXECVE:
		perror("EXECVE ERROR");
		break;
	default:
		break;
	}
}

char **getPathList(char **env)
{
	int i = 0;
	while (env[i] != NULL)
	{
		if (ft_strnstr(env[i], "PATH=/", ft_strlen(env[i])))
			break;
		i++;
	}
	return (ft_split(env[i] + 5, ':'));
}

char *getAbsolutePath(char *command, char **pathList)
{
	char *temp = ft_strjoin("/", command);
	char *temp2 = NULL;
	int i = 0;
	while (pathList[i] != NULL)
	{
		temp2 = ft_strjoin(pathList[i], temp);
		if (access(temp2, F_OK) == 0)
			break ;
		else
			free(temp2);
		i++;
	}
	free(temp);
	if (pathList[i] == NULL)
	{
		perror("command not found");
		return (NULL);
	}
	return (temp2);
}

char **g_env;
int g_stdin;
int g_stdout;

void g_set(char **env)
{
	g_stdin = dup(0);
	g_stdout = dup(1);
	g_env = env;
}

void setInfo(t_info *info, char *argv[], char **pathList)
{
	info->filenameIn = argv[1];
	info->commandIn = ft_split(argv[2], ' ');
	info->commandOut = ft_split(argv[3], ' ');
	info->filenameOut = argv[4];

	info->InAbsolutepath = getAbsolutePath(info->commandIn[0], pathList);
	info->OutAbsolutepath = getAbsolutePath(info->commandOut[0], pathList);
}

int main(int argc, char *argv[], char **env)
{
	// TODO file 인지 폴더인지 먼저
	if (argc != 5)
	{
		print_status(INVALIDARG);
		return (1);
	}
	t_info info;
	ft_memset(&info, 0, sizeof(t_info));
	char **pathlist;
	if (NULL == (pathlist = getPathList(env)))
		return (1);
	g_set(env);
	setInfo(&info, argv, pathlist);
	// printAll(pathlist);
	(void )env;


	if (access(info.filenameIn, R_OK) == -1)
	{
		print_status(NOFILE);
//		exit(1); //파일이 없을 때 종료하면 안됨
	}
	else
	{
		info.fdIn = open(info.filenameIn, O_RDONLY);
		// dup2(info.fdIn, 0);
	}
	
	if (access(info.filenameOut, W_OK) == -1)
	{
		info.fdOut = open(info.filenameOut, O_CREAT | O_RDWR, 0755);
	}
	else
	{
		info.fdOut = open(info.filenameOut, O_CREAT | O_TRUNC | O_RDWR, 0755);
	}

	pid_t pid;
	int pipex[2];
	pipe(pipex);
	if (0 == (pid = fork())) // 자식 프로세스
	{
		dup2(info.fdIn, 0);
		dup2(pipex[1], 1);
		// printf("%d\n", dup2(pipex[1], 1));
		if (-1 == execve(info.InAbsolutepath, info.commandIn, g_env))
		{
			print_status(EXECVE);
			exit(1);
		}
	}
	else
	{
		waitpid(pid, NULL, 0); // 부모 프로세스
	}

	close(pipex[1]);
	if (0 == (pid = fork())) // 자식 프로세스
	{
		close(pipex[1]);
		dup2(pipex[0], 0);
		dup2(info.fdOut, 1);
		if (-1 == execve(info.OutAbsolutepath, info.commandOut, g_env))
		{
			print_status(EXECVE);
			exit(1);
		}
	}
	else
	{
		waitpid(pid, NULL, 0); // 부모 프로세스
	}
	freeInfo(&info);
	dup2(g_stdin, 0);
	dup2(g_stdout, 1);
	return (0);
}

void freeAll(char **list)
{
	if (list == NULL)
		return ;
	int i = 0;
	while (list[i] != NULL)
	{
		if (list[i] != NULL)
			free(list[i]);
		i++;
	}
	free(list);
}

void freeInfo(t_info *info)
{
	free(info->InAbsolutepath);
	free(info->OutAbsolutepath);
	freeAll(info->commandOut);
	freeAll(info->commandIn);
	close(info->fdIn);
	close(info->fdOut);
}

void printAll(char **list)
{
	if (list == NULL)
		return ;
	int i = 0;
	while (list[i] != NULL)
	{
		printf("%s\n", list[i]);
		i++;
	}
}

void read_fd(int fd)
{
	char buffer[1000];
	ft_memset(buffer, 0, sizeof(buffer));
	int temp = read(fd, buffer, 1000);
	if (temp == -1)
	{
		printf("error\n");
		exit(1);
	}
	printf("%s\n", buffer);
}


//  ./pipex_yunslee/pipex "./pipex-tester/assets/deepthought.txt" "cat" "ls" "./pipe-tester/outs/test-xx.txt"
//  < "./pipex-tester/assets/deepthought.txt" cat  | ls > "./pipe-tester/outs/test-xx.txt"
