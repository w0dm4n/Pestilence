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
