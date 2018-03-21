/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_iv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 19:31:12 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/20 19:31:19 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

t_key_iv			*gen_key_iv(char *key, char *iv, int key_len, int iv_len)
{
	t_key_iv		*key_iv = NULL;

	if (!(key_iv = (t_key_iv*)malloc(sizeof(struct s_key_iv))))
		return (NULL);
	if (!(key_iv->key = (char*)malloc(key_len)))
		return (NULL);
	if (!(key_iv->iv = (char*)malloc(iv_len)))
		return (NULL);
	key_iv->key_len = key_len;
	key_iv->iv_len = iv_len;
	memcpy(key_iv->key, key, key_len);
	memcpy(key_iv->iv, iv, iv_len);
	return key_iv;
}


void 				free_key_iv(t_key_iv *key_iv)
{
	if (key_iv->key)
		free(key_iv->key);
	if (key_iv->iv)
		free(key_iv->iv);
	free(key_iv);
}
