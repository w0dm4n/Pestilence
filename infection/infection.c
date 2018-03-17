#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#define ELF_32 0
#define ELF_64 1

size_t	array_length(char **array)
{
	size_t	size;

	size = 0;
	while (array && array[size] != NULL)
		size++;
	return (size);
}

char	**add_element(char **tab, char *element)
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

int		is_directory(const char *filename)
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

int		file_exists(const char *filename)
{
	struct stat	st;

	if (stat(filename, &st) == -1)
		return (0);
	return (1);
}

char	*ft_strnew(size_t size)
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


char	*ft_dstrjoin(char *s1, char *s2, short flag)
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

int		is_elf_type(char *file_path)
{
	int			fd;
	struct stat	st;

	if (stat(file_path, &st) == -1)
		return (0);
	fd = open(file_path, O_RDWR);
	if (fd == -1)
		return (0);
	return (1);
}

char	**get_elf_files(char **files, char *start_path, int elf_type)
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
		if (file_exists(file_path) == 0)
		{
			free(file_path);
			continue ;
		}
		if (is_directory(file_path) == 1)
		{
			dirs = add_element(dirs, strdup(file_path));
			free(file_path);
			continue ;
		}
		if (is_elf_type(file_path) == 1)
		{
			files = add_element(files, strdup(file_path));
		}
		free(file_path);
	}

	int i = 0;
	while (i < array_length(dirs))
	{
		files = get_elf_files(files, dirs[i], elf_type);
		free(dirs[i]);
		i++;
	}
	if (dirs != NULL)
		free(dirs);
	closedir(directory);
	return (files);
}

int		main(void)
{
	char **files = get_elf_files(NULL, ".", ELF_64);
	int i = 0;

	while (i < array_length(files))
	{
		printf("%s\n", files[i]);
		i++;
	}
	return (0);
}
