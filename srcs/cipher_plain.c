/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cipher_plain.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 19:58:44 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/20 19:58:55 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int 			set_plain(t_cipher_plain *cipher_plain, char *plain, int len)
{
	if (!(cipher_plain->plain = (char*)malloc(len)))
		return FALSE;
	cipher_plain->plain_len = len;
	memcpy(cipher_plain->plain, plain, len);
	return TRUE;
}

int				set_cipher(t_cipher_plain *cipher_plain, char *cipher, int len)
{
	if (!(cipher_plain->cipher = (char*)malloc(len)))
		return FALSE;
	cipher_plain->cipher_len = len;
	memcpy(cipher_plain->cipher, cipher, len);
	return TRUE;
}

t_cipher_plain	*get_cipher_plain()
{
	t_cipher_plain *cipher_plain = NULL;

	if (!(cipher_plain = (t_cipher_plain*)malloc(sizeof(struct s_cipher_plain))))
		return (NULL);
	cipher_plain->cipher = NULL;
	cipher_plain->plain = NULL;
	cipher_plain->cipher_len = 0;
	cipher_plain->plain_len = 0;
	return (cipher_plain);
}
