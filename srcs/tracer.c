/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmarinh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/24 23:06:58 by frmarinh          #+#    #+#             */
/*   Updated: 2018/03/24 23:07:07 by frmarinh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all.h"

static int			getTracerPID()
{
    int		status_fd	= 0;
	ssize_t num_read 	= 0;
	char	*pos		= NULL;
	char 	buf[1024];

	if ((status_fd = open("/proc/self/status", O_RDONLY)) == -1) {
		return 0;
	}
    if ((num_read = read(status_fd, buf, (sizeof(buf) - 1))) > 0)
    {
		if ((pos = strstr(buf, "TracerPid:")) != NULL) {
			return (atoi(pos + sizeof("TracerPid:") - 1));
		}
    }
    return 0;
}

static int          getParentPID()
{
    int		status_fd	= 0;
    ssize_t num_read 	= 0;
    char	*pos		= NULL;
    char 	buf[1024];

    if ((status_fd = open("/proc/self/status", O_RDONLY)) == -1) {
        return 0;
    }
    if ((num_read = read(status_fd, buf, (sizeof(buf) - 1))) > 0)
    {
        if ((pos = strstr(buf, "PPid:")) != NULL) {
            return (atoi(pos + sizeof("PPid:") - 1));
        }
    }
    return 0;
}

BOOL				directory_exist(int id)
{
	const char		path[64];
	struct stat		sb;

	memset((char*)&path, 0, 63);
	snprintf((char*)&path, 63, "/proc/%d", id);
	return stat((char*)&path, &sb) == 0 && S_ISDIR(sb.st_mode) == 1;
}

void					get_tracer_status_name(int id, t_tracer *tracer)
{
	const char		path[64];
	int				fd			= 0;
	char			*content	= NULL;
	int				i			= 0;

	memset((char*)&path, 0, 63);
	snprintf((char*)&path, 63, "/proc/%d/status", id);
	if ((fd = open(path, O_RDONLY)) <= 0) {
		return ;
	}
	if ((content = read_contents(fd, 1024)) != NULL) {
		tracer->status = content;
		char *str = strstr(content, "Name:");
		if (str && (str = str + sizeof("Name:"))) {
			while (i < strlen(str) && str[i] != '\n') {
				i++;
			}
			str[i] = '\0';
			tracer->status_name = str;
		}
	}
	return ;
}

char				*get_tracer_cmdline(int id)
{
	const char		path[64];
	int				fd = 0;

	memset((char*)&path, 0, 63);
	snprintf((char*)&path, 63, "/proc/%d/cmdline", id);
	if ((fd = open(path, O_RDONLY)) <= 0) {
		return (NULL);
	}
	return read_contents(fd, 1024);
}

t_tracer			*get_tracer(int id)
{
	t_tracer	*tracer = NULL;
	int			fd		= 0;

	if (!(tracer = (t_tracer*)malloc(sizeof(struct s_tracer))))
		return (NULL);
	tracer->cmdline = get_tracer_cmdline(id);
	tracer->status_name = NULL;
	tracer->status = NULL;

	get_tracer_status_name(id, tracer);
	return tracer;
}

void				free_tracer(t_tracer *tracer)
{
	if (tracer->cmdline)
		free (tracer->cmdline);
	if (tracer->status)
		free(tracer->status);
	free(tracer);
}

BOOL				authorized_tracer(t_tracer *tracer)
{
	char	*tracers[] = {	"bash", "zsh", "csh", "sh", "dash",
	 						"ksh93", "tcsh", "fish", "rbash", "ksh", 0};
	int		i = 0;
	while (tracers[i])
	{
		if (!strcmp(tracer->cmdline, tracers[i])
			|| !strcmp(tracer->status_name, tracers[i])) {
				return TRUE;
			}
		i++;
	}
	return FALSE;
}

BOOL				process_authentifier(t_aes *aes)
{
	int			pid		        = 0;
	t_tracer	*tracer			= NULL;

	if ((pid = getTracerPID()) > 0 || (pid = getParentPID()) > 0) {
		if (directory_exist(pid) &&
		(tracer = get_tracer(pid)) != NULL) {
			if (authorized_tracer(tracer)) {
				aes->valid = TRUE;
			}
			free_tracer(tracer);
			return TRUE;
		}
	}
	return FALSE;
}
