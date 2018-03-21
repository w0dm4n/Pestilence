/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 17:18:39 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/20 17:19:06 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

t_key_iv	*get_key_iv(t_aes *aes)
{
	const unsigned char key[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	const unsigned char iv[] = { 0x00, 0xfa, 0xda, 0x94, 0x44, 0x55, 0x66, 0x7d, 0xbc, 0x99, 0xbc, 0xcd, 0xcc, 0xef, 0xff, 0x1a };
	return (aes->key_iv = gen_key_iv((char*)&key, (char*)&iv, sizeof(key), sizeof(iv)));
}

int main(int argc, char **argv)
{
	t_aes			*aes;
	t_cipher_plain	*cipher_plain;

	if ((aes = load_aes()) != NULL)
	{
		if (get_key_iv(aes) != NULL)
		{
			if ((cipher_plain = get_cipher_plain()) != NULL)
			{
				char bonjour[] = "Hello friend\0";
				set_plain(cipher_plain, (char*)&bonjour, sizeof(bonjour));

				write(1, "clean   chain            : ", 27);
				write(1, cipher_plain->plain, cipher_plain->plain_len);
				printf("\nclean chain length       : %d\n", cipher_plain->plain_len);
				cipher_plain->cipher_len =	encrypt_aes	(
															cipher_plain->plain, cipher_plain->plain_len,
															aes->key_iv->key,
															aes->key_iv->iv,
															cipher_plain->cipher
														);
				write(1, "crypted chain            : ", 27);
				write(1, cipher_plain->cipher, cipher_plain->cipher_len);
				printf("\ncrypted chain length     : %d\n", cipher_plain->cipher_len);
				cipher_plain->plain_len = decrypt_aes	(
															cipher_plain->cipher, cipher_plain->cipher_len,
															aes->key_iv->key,
															aes->key_iv->iv,
															cipher_plain->plain
														);
				write(1, "uncrypted chain          : ", 27);
				write(1, cipher_plain->plain, cipher_plain->plain_len);
				printf("\nuncripted chain length   : %d\n", cipher_plain->plain_len);
				free_cipher_plain(cipher_plain);
			}
		}
		free_aes(aes);
	}
}
