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

#ifndef INFECTION_H
# define INFECTION_H

#define _GNU_SOURCE

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <stdio.h>
# include <sys/mman.h>

# define bool	int
# define true	1
# define false	0

# include <elf.h>

# define ELF_32 0
# define ELF_64 1

/*
** UTILS
*/
size_t	array_length(char **array);
int		is_directory(const char *filename);
char	*ft_strnew(size_t size);
int		file_exists(const char *filename);
char	*ft_dstrjoin(char *s1, char *s2, short flag);

/*
** PROGRAM
*/
char	**get_elf_files(char **files, char *start_path, int elf_type);

#endif
