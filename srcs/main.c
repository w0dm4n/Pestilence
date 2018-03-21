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
	t_aes		*aes = NULL;

	if ((aes = load_aes()) != NULL) {
		if (get_key_iv(aes) != NULL && init_encryption(aes)
		 && init_decryption(aes))
		{
			t_cipher_plain *cipher_plain = get_cipher_plain();
			if (cipher_plain) {
				char bonjour[] = "Nunc fermentum rutrum sapien at convallis. Sed tempus, turpis ultricies ultrices ultrices, tellus ex placerat elit, sit amet volutpat nulla erat sit amet tortor. Etiam porta vel massa et pulvinar. Nunc in eleifend lacus. Maecenas hendrerit feugiat eleifend. Donec mauris eros, congue ut nulla nec, dapibus volutpat nibh. Nullam congue, orci vitae maximus rhoncus, lacus ligula sodales augue, at pharetra erat lacus non erat. Curabitur aliquam tellus est, et suscipit dui semper sed. Duis libero lectus, pulvinar in volutpat at, lobortis at turpis. Vestibulum eu blandit mi.";
				if (set_plain(cipher_plain, (char*)&bonjour, sizeof(bonjour))) {
					if (encrypt_plain_text(aes, cipher_plain) == TRUE
					&& decrypt_cipher_text(aes, cipher_plain) == TRUE)
					{
						printf("Encrypted shit (%s): \n", cipher_plain->cipher);
						int i = 0;
						while (i < cipher_plain->cipher_len) {
							printf("0x%02x ", cipher_plain->cipher[i++]);
						}
						printf("\nDecrypted: %s\n", cipher_plain->plain);
					}
				}
			}
		}
	}

	free_aes(aes);
}
