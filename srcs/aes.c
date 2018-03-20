/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aes.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 18:34:27 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/20 18:34:31 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int 				set_private_key(t_aes *aes, char *key, int len)
{
	return 0;
	// return AES_set_encrypt_key(key, (len * 4), &aes->key);
}

t_aes				*load_aes()
{
	t_aes		*aes = NULL;

	if (!(aes = (t_aes*)malloc(sizeof(struct s_aes))))
		return (NULL);
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();
	return (aes);
}

void				free_aes(t_aes *aes)
{
	ERR_free_strings();
	free(aes);
}
