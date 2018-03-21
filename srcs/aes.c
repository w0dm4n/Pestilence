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

int 				init_encryption(t_aes *aes)
{
	return EVP_CIPHER_CTX_ctrl(aes->ctx, EVP_CTRL_GCM_SET_IVLEN, aes->key_iv->iv_len, NULL) == 1
		&& EVP_EncryptInit_ex(aes->ctx, NULL, NULL, aes->key_iv->key, aes->key_iv->iv) == 1;
}

t_aes				*load_aes()
{
	t_aes		*aes = NULL;

	OpenSSL_add_all_algorithms();
	if (!(aes = (t_aes*)malloc(sizeof(struct s_aes))))
		return (NULL);
	if (!(aes->ctx = EVP_CIPHER_CTX_new()))
		return (NULL);
	if (!EVP_EncryptInit_ex(aes->ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		return (NULL);
	aes->key_iv = NULL;
	return (aes);
}

void				free_aes(t_aes *aes)
{
	if (aes->key_iv)
		free_key_iv(aes->key_iv);
	EVP_CIPHER_CTX_free(aes->ctx);
	free(aes);
}

void				encrypt_plain_text(t_aes *aes, t_cipher_plain *cipher_plain)
{
	int len = 0;
	if (!EVP_EncryptUpdate(aes->ctx, cipher_plain->cipher, &len, cipher_plain->plain, cipher_plain->plain_len))
		return;
	cipher_plain->cipher_len = len;
}

void				decrypt_cipher_text(t_aes *aes, t_cipher_plain *cipher_plain)
{
	int len = 0;
	char *test = malloc(sizeof(cipher_plain->plain_len));

	if (!EVP_DecryptUpdate(aes->ctx, test, &len, cipher_plain->cipher, cipher_plain->cipher_len))
		return;
	printf("%s\n", test);
}
