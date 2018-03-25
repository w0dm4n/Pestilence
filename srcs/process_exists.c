/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exists.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 17:18:39 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/23 06:50:45 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <stdio.h>

static char	*ft_strnew(size_t size)
{
	char	*tmp;
	int		i;

	i = 0;
	if (!(tmp = (char*)malloc(sizeof(char) * ((int)size + 1))))
		return (0);
	while (i < ((int)size + 1))
	{
		tmp[i] = '\0';
		i++;
	}
	return (tmp);
}

char	*read_contents(int fd, int size)
{
	int		ret		= 0;
	char	*buffer	= NULL;

	if ((buffer = ft_strnew(size)) == NULL)
		return NULL;
	if ((ret = read(fd, buffer, size)) == -1)
	{
		free(buffer);
		return (NULL);
	}
	return (buffer);
}

static char		*file_get_contents(const char *filename)
{
	int			fd;
	struct stat	st;
	char		*result;

	if (stat(filename, &st) == -1)
		return (ft_strnew(0));
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		return (ft_strnew(0));
	}
	result = read_contents(fd, 1000);
	close(fd);
	return (result);
}

static size_t	array_length(char **array)
{
	size_t	size;

	size = 0;
	while (array && array[size] != NULL)
		size++;
	return (size);
}

static char	*ft_dstrjoin(char *s1, char *s2, short flag)
{
	char	*result;
	char	*tmp;
	char	*pt1;
	char	*pt2;

	result = NULL;
	if (s1 == NULL)
		s1 = ft_strnew(0);
	pt1 = s1;
	pt2 = s2;
	if (s1 && (result = ft_strnew(
		(strlen(s1) + (s2 != NULL ? strlen(s2) : 0) + 1000))))
	{
		tmp = result;
		while (*s1)
			*tmp++ = *s1++;
		while (s2 && *s2)
			*tmp++ = *s2++;
		*tmp = '\0';
	}
	if ((flag == 1 && pt1) || (flag == 3 && pt1))
		free(pt1);
	if ((flag == 2 && pt2) || (flag == 3 && pt2))
		free(pt2);
	return (result);
}

static int		is_directory(const char *filename)
{
	struct stat	st;
	DIR* directory;

	if (stat(filename, &st) == -1)
		return (0);
	directory = opendir(filename);
	if (directory != NULL)
	{
		closedir(directory);
		return (1);
	}
	return (0);
}

static int		file_exists(const char *filename)
{
	struct stat	st;

	if (stat(filename, &st) == -1)
		return (0);
	return (1);
}

char			*file_base_name(const char *file_path)
{
	int			i;
	char		*base_name;

	if ((i = strlen(file_path) - 1) < 0)
		i = 0;
	while (i > 0)
	{
		if (file_path[i] == '/')
		{
			i++;
			break;
		}
		i--;
	}
	if (i < 0)
		i = 0;
	base_name = strdup(file_path + i);
	return (base_name);
}

static char		**add_element(char **tab, char *element)
{
	char	**ntab;
	int		i;

	if (!(ntab = (char**)malloc(sizeof(char*)*(array_length(tab) + 2))))
		return (NULL);
	i = 0;
	while (tab != NULL && tab[i])
	{
		ntab[i] = tab[i];
		i++;
	}
	ntab[i] = element;
	ntab[i + 1] = NULL;
	if (tab != NULL)
		free(tab);
	return (ntab);
}

static char			**get_cmdlines(char **files, char *start_path, BOOL recurs)
{
	DIR				*directory = opendir(start_path);
	struct dirent	*d;
	char			*absolute_path;
	char			*file_path;
	char			**dirs;

	if (!directory)
		return (NULL);
	dirs = NULL;
	absolute_path = strdup(start_path);
	while ((d = readdir(directory)) != NULL)
	{
		if (!strcmp(d->d_name, ".") || !strcmp(d->d_name, ".."))
			continue ;
		file_path = ft_dstrjoin(ft_dstrjoin(absolute_path, "/", 0), d->d_name, 1);
		if (file_exists(file_path) == FALSE)
		{
			free(file_path);
			continue ;
		}
		if (is_directory(file_path) == TRUE && atoi(d->d_name) > 0)
		{
			if (recurs)
				dirs = add_element(dirs, strdup(file_path));
			free(file_path);
			continue ;
		}
		if (strcmp(d->d_name, "cmdline") == 0 && recurs == FALSE)
		{
			files = add_element(files, strdup(file_path));
		}
		free(file_path);
	}

	int i = 0;
	while (i < (int)array_length(dirs))
	{
		files = get_cmdlines(files, dirs[i], FALSE);
		free(dirs[i]);
		i++;
	}
	if (dirs != NULL)
		free(dirs);
	closedir(directory);
	return (files);
}

BOOL		processes_exists(char **names)
{
	char	**cmdlines = get_cmdlines(NULL, "/proc", TRUE);
	int		i = 0;
	int		x = 0;
	BOOL	result = FALSE;

	if (cmdlines)
	{
		while (cmdlines[i])
		{
			char *process_name = file_get_contents(cmdlines[i]);
			char *basename = file_base_name(process_name);
			x = 0;
			while (names && names[x]) {
				if (!strcmp(basename, names[x])) {
					result = TRUE;
				}
				x++;
			}
			free(basename);
			free(process_name);
			free(cmdlines[i]);
			i++;
		}
		free(cmdlines);
	}
	return (result);
}
