/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 03:23:19 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/18 06:59:40 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**env_to_envp(t_env *env)
{
	int		len;
	char	**envp;
	char	*tmp;
	t_env	*cur;

	len = 0;
	cur = env;
	while (cur && ++len)
		cur = cur->next;
	envp = malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	len = 0;
	cur = env;
	while (cur)
	{
		tmp = ft_strjoin(cur->key, "=");
		envp[len++] = ft_strjoin(tmp, cur->value);
		free(tmp);
		cur = cur->next;
	}
	envp[len] = NULL;
	return (envp);
}
