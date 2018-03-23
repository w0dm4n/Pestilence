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

int isDebuggerPresent(t_aes *aes)
{
    char buf[1024];
    int debugger_present = 0;

    int status_fd = open("/proc/self/status", O_RDONLY);
    if (status_fd == -1)
        return 0;

    ssize_t num_read = read(status_fd, buf, sizeof(buf)-1);

    if (num_read > 0)
    {
		printf("%s\n", buf);
		// static const char TracerPid[] = "TracerPid:";
        // char *tracer_pid;
        //
        // buf[num_read] = 0;
        // tracer_pid    = strstr(buf, TracerPid);
        // if (tracer_pid)
        //     debugger_present = !!atoi(tracer_pid + sizeof(TracerPid) - 1);
    } else {
		printf("???\n");
	}
	if (!debugger_present)
		aes->valid = TRUE;
    return debugger_present;
}

void anti_debug() {

	int child_pid = fork();

    if (child_pid == 0)
    {
        int ppid = getppid();
        int status;

        if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) == 0)
        {
            waitpid(ppid, &status, 0);

            ptrace(PTRACE_CONT, ppid, NULL, NULL);
			_exit(0);
            while (waitpid(ppid, &status, 0)) {

                if (WIFSTOPPED(status)) {
					ptrace(PTRACE_CONT, ppid, NULL, NULL);
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
	// if (getenv("LD_PRELOAD") != NULL) {
	// 	return FALSE;
	// }
	//anti_debug();
	// void *handle = dlopen (0, RTLD_NOW | RTLD_GLOBAL);
    //
	// if (handle) {
	// 	void *func = dlsym(handle, "ptrace");
	// 	if (func != NULL) {
	// 		printf("Hey\n");
	// 	}
	// }
	// if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
	// 	printf("AH\n");
	// 	return FALSE;
	// }
	// if (isDebuggerPresent(aes)) {
	// 	return FALSE;
	// }
	aes->valid = TRUE;
	return TRUE;
}
