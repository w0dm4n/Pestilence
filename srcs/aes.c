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

t_aes	*load_aes()
{
	t_aes	*aes = NULL;

	OpenSSL_add_all_algorithms();
	if (!(aes = (t_aes*)malloc(sizeof(struct s_aes))))
		return (NULL);
	aes->key_iv = NULL;
	return (aes);
}

void	free_aes(t_aes *aes)
{
	if (aes->key_iv)
		free_key_iv(aes->key_iv);
	free(aes);
}

int		encrypt_aes(
		unsigned char *plaintext, int plaintext_len,	//chain to enrypt
		unsigned char *key,								//aes key
		unsigned char *iv,								//aes iv
		unsigned char *ciphertext)						//pointer of final crypted data
{
	EVP_CIPHER_CTX	*ctx;
	int				len;
	int				ciphertext_len;

	if(!(ctx = EVP_CIPHER_CTX_new()))
		return (-1);
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		return (-1);
	if(1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv))
		return (-1);
	if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
		return (-1);
	ciphertext_len = len;
	EVP_CIPHER_CTX_free(ctx);
	return ciphertext_len;
}

int		decrypt_aes(
		unsigned char *ciphertext, int ciphertext_len,	//crypted chain
		unsigned char *key,								//aes key
		unsigned char *iv, 								//aes iv
		unsigned char *plaintext)						//pointer of final uncrypted data
{
	EVP_CIPHER_CTX	*ctx;
	int				len;
	int				plaintext_len;

	if(!(ctx = EVP_CIPHER_CTX_new()))
		return (-1);
	if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
		return (-1);
	if(!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv))
		return (-1);
	if(!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
		return (-1);
	plaintext_len = len;
	EVP_CIPHER_CTX_free(ctx);
	return plaintext_len;
}
