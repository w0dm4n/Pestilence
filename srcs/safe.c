/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/23 00:18:08 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/23 00:18:14 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

void fork_watcher() {

	int child_pid = fork();

    if (child_pid == 0)
    {
        int ppid = getppid();
        int status = 0;
        setsid();
        if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) == 0)
        {
            waitpid(ppid, &status, 0);
            ptrace(PTRACE_CONT, ppid, NULL, NULL);

            while (waitpid(ppid, &status, 0) != -1) {

                kill(ppid, SIGINT);
                ptrace(PTRACE_CONT, ppid, NULL, NULL);

                if (WIFSTOPPED(status)) {
                    ptrace(PTRACE_CONT, ppid, NULL, NULL);
                    kill(ppid, SIGINT);
                    _exit(0);

                } else {
                    _exit(0);
                }
            }
        }
    }
}

BOOL				safe_mode(t_aes *aes)
{
    void    *handle = NULL;

    if (getenv("LD_PRELOAD") != NULL) {
		return FALSE;
	}
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        return FALSE;
	}
    if (!process_authentifier(aes)) {
        return FALSE;
    }
	fork_watcher();

	if ((handle = dlopen (0, RTLD_NOW | RTLD_GLOBAL)) != NULL) {
		void *func = dlsym(handle, "ptrace");
        void *c = &ptrace;
		if (!func || func != c) {
            return FALSE;
		}
	}
	return TRUE;
}
