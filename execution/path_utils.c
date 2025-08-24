/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 10:38:03 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/24 10:49:56 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_path_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*find_bin(char *cmd, char **paths)
{
	int		i;
	char	*tmp;
	char	*full;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
			return (NULL);
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!full)
			return (NULL);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	return (NULL);
}

char *find_cmd_path(char *cmd, char **envp)
{
    char *path;
    char **paths;
    char *full;

    if (access(cmd, X_OK) == 0)
        return (ft_strdup(cmd));
    if (!envp)
        return (NULL);
    path = get_path_envp(envp);
    if (!path)
        return (NULL);
    paths = ft_split(path, ':');
    if (!paths)
        return (NULL);
    full = find_bin(cmd, paths);
    free_split(paths);
    return (full);
}

