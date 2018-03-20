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
