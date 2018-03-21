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
void				free_cipher_plain(t_cipher_plain *cipher_plain);
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
	t_key_iv		*key_iv;
}					t_aes;

t_aes				*load_aes(void);

int					encrypt_aes	(
									unsigned char *plaintext, int plaintext_len,	//chain to enrypt
									unsigned char *key,								//aes key
									unsigned char *iv, 								//aes iv
									unsigned char *ciphertext						//pointer of final crypted data
								);
int					decrypt_aes	(
									unsigned char *ciphertext, int ciphertext_len,	//crypted chain
									unsigned char *key,								//aes key
									unsigned char *iv, 								//aes iv
									unsigned char *plaintext						//pointer of final uncrypted data
								);
#endif
