/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:10:49 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/22 11:51:23 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../libft/libft.h"
# include "minishell.h"

/* ========================= STRING UTILS ======================== */
int		ft_strcmp(const char *s1, const char *s2);
char	*builtin_remove_quotes(char *str);
int		ft_is_numeric(const char *str);
int		builtin_is_valid_key(char *key);
int		only_spaces(const char *str);

/* ========================= BUILTIN HELPERS ===================== */
void	builtin_print_export_env(t_env *env);
void	builtin_parse_export_arg(char *arg, char **key, char **value);
void	free_split(char **split);
char	*builtin_create_env_var(const char *key, const char *value);

/* ========================= BUILTIN FUNCTIONS =================== */
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_env(t_env *env_list);

int		ft_export(char **args, t_env **env);
int		ft_unset(char **args, t_env **env);

int		ft_cd(char **args, t_env **env_list);
int		ft_exit(char **args, int *status);

/* ========================= EXEC HELPERS ======================== */
int		is_builtin(char *cmd);                  /* <--- add this */
int		run_builtin(char **args, t_env **env, int *status); /* <--- add this */
/* Environment helper */
void    env_unset(t_env **env, char *key);

void	env_set(t_env **env, const char *key, const char *value);
char	*env_get(t_env *env, const char *key);
t_env	*env_new(char *key, char *value);
void	free_env_list(t_env *env);
void	add_env(t_env **env, char *key, char *value);

t_env	*env_new_from_entry(char *envp_entry);


#endif
