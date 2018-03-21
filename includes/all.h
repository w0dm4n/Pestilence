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

#define TRUE		1
#define FALSE		0
#define BOOL		int

/*
**	CIPHER PLAIN
*/

typedef struct		s_cipher_plain
{
	char			*cipher;
	char			*plain;
	int				cipher_len;
	int				plain_len;
}					t_cipher_plain;

t_cipher_plain		*get_cipher_plain();
int 				set_plain(t_cipher_plain *cipher_plain, char *plain, int len);

/*
**	KEY_IV
*/

typedef struct		s_key_iv
{
	char			*key;
	char			*iv;
	int				key_len;
	int				iv_len;
}					t_key_iv;


t_key_iv			*gen_key_iv(char *key, char *iv, int key_len, int iv_len);
void 				free_key_iv(t_key_iv *key_iv);

/*
**	AES
*/

#define AES_SUCCESS	0
typedef struct		s_aes
{
	EVP_CIPHER_CTX	*ctx_encryption;
	EVP_CIPHER_CTX	*ctx_decryption;
	t_key_iv		*key_iv;
}					t_aes;


t_aes				*load_aes(void);
void				free_aes(t_aes *aes);
int 				init_encryption(t_aes *aes);
int 				init_decryption(t_aes *aes);

BOOL				encrypt_plain_text(t_aes *aes, t_cipher_plain *cipher_plain);
BOOL				decrypt_cipher_text(t_aes *aes, t_cipher_plain *cipher_plain);
#endif
