/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 18:24:02 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/20 18:24:28 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALL_H
#define ALL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>


/*
**	AES
*/

#define AES_SUCCESS	0
typedef struct		s_aes
{
	EVP_CIPHER_CTX	*ctx;
	char			key_raw[32];
	char			iv_raw[16];
}					t_aes;


t_aes				*load_aes(void);
int 				set_private_key(t_aes *aes, char *key, int len);

#endif
