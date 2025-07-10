/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 12:31:51 by mbouchri          #+#    #+#             */
/*   Updated: 2025/07/10 09:15:34 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns the length of a NULL-terminated env array
int	env_len(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

// Creates a t_env* linked list from a char **envp array
t_env	*copy_env(char **envp)
{
	t_env	*head;
	t_env	*node;
	char	*equal;
	char	*key;
	char	*value;

	head = NULL;
	while (*envp)
	{
		equal = ft_strchr(*envp, '=');
		if (equal)
		{
			key = ft_substr(*envp, 0, equal - *envp);
			value = ft_strdup(equal + 1);
		}
		else
		{
			key = ft_strdup(*envp);
			value = NULL;
		}
		node = malloc(sizeof(t_env));
		if (!node)
			return (free_env_list(head), NULL);
		node->key = key;
		node->value = value;
		node->next = NULL;
		add_node_back(&head, node);
		envp++;
	}
	return (head);
}
