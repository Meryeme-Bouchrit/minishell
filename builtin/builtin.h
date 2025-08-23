/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouchri <mbouchri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:10:49 by mbouchri          #+#    #+#             */
/*   Updated: 2025/08/23 19:56:05 by mbouchri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "../libft/libft.h"
# include "minishell.h"

/* ========================= STRING UTILS ======================== */
int		ft_strcmp(const char *s1, const char *s2);
int		ft_is_numeric(const char *str);
int		is_valid_key(char *key);
int		only_spaces(const char *str);
void	free_split(char **split);

/* ========================= BUILTIN HELPERS ===================== */
void	print_export_env(t_env *env);
void	parse_export_arg(char *arg, char **key, char **value);

/* ========================= BUILTIN FUNCTIONS =================== */
int		ft_echo(char **args);
int		ft_pwd(void);
int		ft_env(t_env *env_list);
int		ft_export(char **args, t_env **env);
int		ft_unset(char **args, t_env **env);
int		ft_cd(char **args, t_env **envp);
int		ft_exit(char **args, int *status);

/* ========================= EXEC HELPERS ======================== */
int		is_builtin(char *cmd);
int		run_builtin(char **args, t_env **envp, int *status);

/* ========================= ENVIRONMENT ========================= */
t_env	*env_new(char *key, char *value);
void	env_add(t_env **env, char *key, char *value);
void	env_free_all(t_env *env);
t_env	*env_from_entry(char *envp_entry);
t_env	*env_dup(char **envp);
t_env	*env_find(t_env *env, const char *key);
char	*env_get(t_env *env, const char *key);
void	env_set(t_env **env, const char *key, const char *value);
void	env_unset(t_env **env, char *key);

int		process_export_arg(char *arg, t_env **envp);
char	*get_home(t_env *envp);
char	*get_oldpwd(t_env *envp);
int		change_dir(char *path, t_env **envp, char *oldpwd);

#endif
