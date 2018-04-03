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

void	       *write_elf(t_elf *elf)
{
	void	*ptr;

	if (!(ptr = (void*)malloc(elf->len)))
		return (NULL);
	bzero(ptr, elf->len);
	memcpy(ptr, elf->buffer, elf->len);
	return (ptr);
}
