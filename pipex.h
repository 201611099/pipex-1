/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunslee <yunslee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 03:36:50 by yunslee           #+#    #+#             */
/*   Updated: 2021/07/15 05:29:23 by yunslee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define IN 0
#define OUT 1

typedef enum e_status
{
	NOFILE,
	INVALIDARG,
	EXECVE
}			t_status;

typedef struct s_info
{
	char *filenameIn;
	char **commandIn;
	char *InAbsolutepath;
	int fdIn;

	char **commandOut;
	char *filenameOut;
	char *OutAbsolutepath;
	int fdOut;
}				t_info;

void freeInfo(t_info *info);
void freeAll(char **list);
void printAll(char **list);
void read_fd(int fd);


#endif