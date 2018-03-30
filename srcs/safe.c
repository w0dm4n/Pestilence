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
#include <errno.h>

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

    char *forbiddens[] = { "cat", "top", "htop", "gdb", "netcat", "ps", "valgrind", 0};
    if (processes_exists((char**)&forbiddens[0])) {
        return FALSE;
    }
	if ((handle = dlopen (0, RTLD_NOW | RTLD_GLOBAL)) != NULL) {
		void *func = dlsym(handle, "ptrace");
        void *c = &ptrace;
		if (!func || func != c) {
            return FALSE;
		}
	}
	return TRUE;
}
