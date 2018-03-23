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
	return EVP_CIPHER_CTX_ctrl(aes->ctx_encryption, EVP_CTRL_GCM_SET_IVLEN, aes->key_iv->iv_len, NULL) == 1
		&& EVP_EncryptInit_ex(aes->ctx_encryption, NULL, NULL, aes->key_iv->key, aes->key_iv->iv) == 1;
}

int 				init_decryption(t_aes *aes)
{
	return EVP_CIPHER_CTX_ctrl(aes->ctx_decryption, EVP_CTRL_GCM_SET_IVLEN, aes->key_iv->iv_len, NULL) == 1
		&& EVP_DecryptInit_ex(aes->ctx_decryption, NULL, NULL, aes->key_iv->key, aes->key_iv->iv) == 1;
}

t_aes				*load_aes()
{
	t_aes		*aes = NULL;

	OpenSSL_add_all_algorithms();
	if (!(aes = (t_aes*)malloc(sizeof(struct s_aes))))
		return (NULL);
	if (!(aes->ctx_encryption = EVP_CIPHER_CTX_new()))
		return (NULL);
	if (!(aes->ctx_decryption = EVP_CIPHER_CTX_new()))
		return (NULL);
	if (!EVP_EncryptInit_ex(aes->ctx_encryption, EVP_aes_256_gcm(), NULL, NULL, NULL))
		return (NULL);
	if (!EVP_DecryptInit_ex(aes->ctx_decryption, EVP_aes_256_gcm(), NULL, NULL, NULL))
		return (NULL);
	aes->key_iv = NULL;
	aes->valid = FALSE;
	return (aes);
}

void				free_aes(t_aes *aes)
{
	if (aes->key_iv)
		free_key_iv(aes->key_iv);
	EVP_CIPHER_CTX_free(aes->ctx_encryption);
	EVP_CIPHER_CTX_free(aes->ctx_decryption);
	free(aes);
}

BOOL				encrypt_plain_text(t_aes *aes, t_cipher_plain *cipher_plain)
{
	int len = 0;
	if (cipher_plain->cipher != NULL) {
		free(cipher_plain->cipher);
		cipher_plain->cipher = NULL;
	}
	if (!(cipher_plain->cipher = (char*)malloc(cipher_plain->plain_len)))
		return FALSE;
	if (!EVP_EncryptUpdate(aes->ctx_encryption, cipher_plain->cipher, &len, cipher_plain->plain, cipher_plain->plain_len))
		return FALSE;
	cipher_plain->cipher_len = len;
	return (cipher_plain->cipher_len > 0) ? TRUE : FALSE;
}

BOOL				decrypt_cipher_text(t_aes *aes, t_cipher_plain *cipher_plain)
{
	int len = 0;
	if (cipher_plain->plain != NULL) {
		free(cipher_plain->plain);
		cipher_plain->plain = NULL;
	}
	if ((cipher_plain->plain = (char*)malloc(cipher_plain->cipher_len)) == NULL)
		return FALSE;
	if (!EVP_DecryptUpdate(aes->ctx_decryption, cipher_plain->plain, &len, cipher_plain->cipher, cipher_plain->cipher_len))
		return FALSE;
	cipher_plain->plain_len = len;
	return (len > 0) ? TRUE : FALSE;
}
