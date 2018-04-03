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

t_elf		*new_elf(void)
{
	t_elf	*elf;

	if (!(elf = (t_elf*)malloc(sizeof(struct s_elf))))
		return (NULL);
	elf->header = NULL;
	elf->is_64 = false;
	elf->little_endian = false;
	elf->big_endian = false;
	elf->len = 0;
	elf->buffer = NULL;
	elf->name = NULL;
	return elf;
}

void		destruct_elf(t_elf *elf)
{
	if (elf->header != NULL)
		free(elf->header);
	if (elf->name != NULL)
		free(elf->name);
	if (elf->buffer != NULL)
		munmap(elf->buffer, elf->len);
	free(elf);
}
