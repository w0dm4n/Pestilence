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

int		is_directory(const char *filename)
{
	struct stat	st;
	DIR* directory;

	if (stat(filename, &st) == -1)
		return (0);
	directory = opendir(filename);
	if (directory != NULL)
	{
		closedir(directory);
		return (1);
	}
	return (0);
}
