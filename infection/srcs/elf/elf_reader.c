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

t_elf		*load_section_name(t_elf *elf)
{
	struct elf64_shdr	*section;
	char				*string_tab;

    section = (struct elf64_shdr*) (elf->buffer + elf->header->e_shoff);
    string_tab = elf->buffer + section[elf->header->e_shstrndx].sh_offset;
	elf->string_tab = string_tab;
	return (elf);
}

static void			add_segment(t_elf *elf, t_segment *segment)
{
	t_segment	*tmp;

	tmp = elf->segments;
	if (elf->segments != NULL)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = segment;
		segment->prev = tmp;
	}
	else
		elf->segments = segment;
}

t_elf		*load_segments(t_elf *elf)
{
	struct elf64_phdr	*segments;
	struct s_segment	*segment;
	int					i;

	i = 0;
	if (elf->buffer == NULL)
		return (NULL);
	segments = elf->buffer + elf->header->e_phoff;
	while (i < elf->header->e_phnum)
	{
		if (segments[i].p_type == 0)
		{
			i++;
			continue ;
		}
		if (!(segment = malloc(sizeof(struct s_segment))))
			return (NULL);
		if (!(segment->data = malloc(sizeof(struct elf64_phdr))))
			return (NULL);
		//Load Segment Header
		bzero(segment->data, sizeof(struct elf64_phdr));
		memcpy(segment->data, &segments[i], sizeof(struct elf64_phdr));
		segment->prev = NULL;
		segment->next = NULL;
		segment->id = i;
		add_segment(elf, segment);
		i++;
	}
	return (elf);
}

static void			add_section(t_elf *elf, t_section *section)
{
	t_section *tmp;

	tmp = elf->sections;
	if (elf->sections != NULL)
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = section;
		section->prev = tmp;
	}
	else
		elf->sections = section;
}

t_elf	   *load_sections(t_elf *elf)
{
	struct elf64_shdr	*sections;
	struct s_section	*section;
	int					i;

	i = 0;
	if (elf->buffer == NULL)
		return (NULL);
	sections = (struct elf64_shdr*) (elf->buffer + elf->header->e_shoff);
	while (i < elf->header->e_shnum)
	{
		if (sections[i].sh_offset == 0)
		{
			i++;
			continue ;
		}
		if (!(section = malloc(sizeof(struct s_section))))
			return (NULL);
		if (!(section->data = malloc(sizeof(struct elf64_shdr))))
			return (NULL);
		//Load Section Header
		bzero(section->data, sizeof(struct elf64_shdr));
		memcpy(section->data, &sections[i], sizeof(struct elf64_shdr));
		//Load Section Content
		section->prev = NULL;
		section->next = NULL;
		section->name = strdup(elf->string_tab + sections[i].sh_name);
		if (!(section->content = malloc(sections[i].sh_size)))
			return (NULL);
		bzero(section->content, sections[i].sh_size);
		if (strcmp(section->name, ".bss") != 0)
		{
			memcpy(section->content, elf->buffer + sections[i].sh_offset, sections[i].sh_size);
		}
		else
		{
			elf->len += sections[i].sh_size;
		}
		section->parent = elf->get_segment_by_section(elf, section);
		add_section(elf, section);
		i++;
	}
	return (elf);
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
	elf->name = strdup(file_name);
	load_elf_header(elf);
	if (is_elf_file(elf))
	{
		if (elf->is_64)
		{
			load_section_name(elf);
			load_segments(elf);
			load_sections(elf);
			munmap(elf->buffer, lenmap);
			if (elf->sections == NULL || elf->segments == NULL)
			{
				destruct_elf(elf);
				return (NULL);
			}
		}
		return (elf);
	}
	destruct_elf(elf);
	close(fd);
	return (NULL);
}
