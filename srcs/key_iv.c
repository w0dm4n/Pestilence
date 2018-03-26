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

t_key_iv			*get_key_iv(t_aes *aes)
{
	const unsigned char key[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	const unsigned char iv[] = { 0x00, 0xfa, 0xda, 0x94, 0x44, 0x55, 0x66, 0x7d, 0xbc, 0x99, 0xbc, 0xcd, 0xcc, 0xef, 0xff, 0x1a };
	return (aes->key_iv = gen_key_iv((char*)&key, (char*)&iv, sizeof(key), sizeof(iv)));
}

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
