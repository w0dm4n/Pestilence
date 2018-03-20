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

void     *write_header(void *ptr, t_elf *elf)
{
	elf->header->e_phnum = elf->get_nbr_segments(elf->segments);
	elf->header->e_shnum = elf->get_nbr_sections(elf->sections);
	elf->header->e_phoff = sizeof(struct elf64_hdr);
	elf->header->e_shoff = elf->get_offset_sections(elf);
	memcpy(ptr, elf->header, sizeof(struct elf64_hdr));
	return (ptr);
}

bool	   write_segments(void *ptr, t_elf *elf)
{
	t_segment	*tmp;
	int			offset;

	tmp = elf->segments;
	offset = 0;
	while (tmp)
	{
		memcpy(ptr + elf->header->e_phoff + offset, tmp->data, sizeof(struct elf64_phdr));
		offset += sizeof(struct elf64_phdr);
		tmp = tmp->next;
	}
	return (true);
}

bool     write_sections(void *ptr, t_elf *elf, bool write_dynamic)
{
	t_section	*tmp;
	int			offset;

	tmp = elf->sections;
	offset = 0;
	while (tmp)
	{
		if (write_dynamic == true)
			memcpy(ptr + elf->header->e_shoff + offset, tmp->data, sizeof(struct elf64_shdr));
		memcpy(ptr + tmp->data->sh_offset, tmp->content, tmp->data->sh_size);
		offset += sizeof(struct elf64_shdr);
		tmp = tmp->next;
	}
	return (true);
}

void	       *write_elf(t_elf *elf)
{
	void	*ptr;

	if (!(ptr = (void*)malloc(elf->len)))
		return (NULL);
	bzero(ptr, elf->len);

	write_header(ptr, elf);
	write_segments(ptr, elf);
	write_sections(ptr, elf, false);
	return (ptr);
}
