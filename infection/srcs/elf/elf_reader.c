/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/17 19:55:13 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/17 19:55:17 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infection.h"

void			*ft_mmap(int fd, size_t size)
{
	return (mmap(0, size, PROT_READ, MAP_SHARED, fd, 0));
}

static void		   load_elf_magic(t_elf *elf)
{
	char	*value = NULL;
	char	*save_pointer = NULL;
	int		i = 0;

	if (!(value = malloc(MAGIC_LEN * 2)))
		return ;
	save_pointer = value;
	bzero(elf->magic, MAGIC_LEN * 2);
	while (i < MAGIC_LEN)
	{
		asprintf(&value, "%x", elf->header->e_ident[i++]);
		strncat((char*) elf->magic, value, 2);
		value += 2;
	}
	free(save_pointer);
}

static void		   load_elf_arch(t_elf *elf)
{
	int				arch = elf->header->e_ident[4];
	elf->is_64 = (arch == ARCH_64) ? true : false;
}

static void		   load_elf_bits_order(t_elf *elf)
{
	int				endian = elf->header->e_ident[5];

	if (endian == L_ENDIAN)
		elf->little_endian = true;
	else if (endian == B_ENDIAN)
		elf->big_endian = true;
}

static bool		   is_elf_file(const t_elf *elf)
{
	return (strcmp(ELF_MAGIC_HEXA, (char*)elf->magic) == 0);
}

static bool		   load_elf_header(t_elf *elf)
{
	struct elf64_hdr* header = (struct elf64_hdr*)elf->buffer;

	if (!(elf->header = malloc(sizeof(struct elf64_hdr))))
		return (false);
	bzero(elf->header, sizeof(struct elf64_hdr));
	memcpy(elf->header, header, sizeof(struct elf64_hdr));

	load_elf_magic(elf);
	load_elf_arch(elf);
	load_elf_bits_order(elf);
	return (true);
}

t_elf			*read_elf(const char *file_name)
{
	t_elf	*elf;
	int		fd;
	long long int lenmap;

	if (!(elf = new_elf()))
		return (NULL);
	if ((fd = open(file_name, O_RDONLY)) == -1)
		return (NULL);
	if ((lenmap = lseek(fd, 0, SEEK_END)) <= 0)
	{
		close(fd);
		return (NULL);
	}
	elf->len = lenmap;
	if (!(elf->buffer = ft_mmap(fd, lenmap)))
	{
		close(fd);
		return (NULL);
	}
	if (find_pattern(elf->buffer, lenmap) != 0) {
		destruct_elf(elf);
		close(fd);
		return (NULL);
	}
	elf->name = strdup(file_name);
	load_elf_header(elf);
	if (is_elf_file(elf))
	{
		if (elf->is_64)
		{
			return (elf);
		}
	}
	destruct_elf(elf);
	close(fd);
	return (NULL);
}
