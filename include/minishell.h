/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiguet <bfiguet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/26 09:59:34 by thsamina          #+#    #+#             */
/*   Updated: 2023/10/31 16:16:38 by bfiguet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//INCLUDE//
# include "libft.h"

# include <error.h>
# include <unistd.h>
# include <errno.h>
# include <stdbool.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>

//global
extern int	g_sig;

typedef enum s_sig_mode
{
	DEFAULT,
	HEREDOC,
	MUTE,
	CMD
}	t_sig_mode;

typedef struct s_fd
{
	int	a;
	int	b;
}		t_fd;

typedef union s_pipe_fd
{
	t_fd	frame;
	int		raw[2];
}			t_pipe_fd;

typedef struct s_export
{
	char			*name;
	char			*value;
	struct s_export	*next;
}					t_export;

typedef struct s_token
{	
	char			*value;
	char			**args;
	char			**red;
	int				fds[2];
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_data
{
	char			*input;
	char			*trimmed;
	int				i;
	int				fd_in;
	int				fd_out;
	int				*pid;
	t_pipe_fd		*pipe_fd;
	struct s_token	*head;
}					t_data;

typedef struct s_params
{
	char			**env;
	t_export		*export;
	t_data			*data;
	int				old_fd[2];
	int				error;
	int				exit_code;
}					t_params;

/////////////MAIN////////////
int			ft_err_malloc(t_data *data, t_params *params);
int			minishell(t_data *data, t_params *params);
int			prompt(t_data *data, t_params *params);

////////////SIGNAL/////////////
void		re_prompt(int sig);
void		back_slash(int sig);
void		ctrl_c_heredoc(int sig);
void		ft_signal(t_sig_mode sig);
void		check_exit_status(t_params *params);

/////////////BUILTIN//////////////
// CD//
int			ft_cd(char **arg, t_params *params);
// ECHO//
int			print_export(t_params *params);
int			ft_echo(char **arg);
// ENV//
int			ft_env(char **arg, t_params *params);
int			if_new_env(char *arg, t_params *params);
// EXIT//
int			ft_exit(char **arg, int i, t_params *params, int *old_fd);
// EXPORT//
int			place_new(char *arg, t_export *new, t_export *new2,
				t_params *params);
int			ft_export(char **arg, t_params *params);
//EXPORT_BIS//
t_export	*new_export(char *str);
// PWD //
int			ft_pwd(char **arg);
// UNSET//
int			ft_unset(char **arg, t_params *params);

/////////////EXEC//////////////
// RUN_CMD//
int			run_cmd(t_token *token, t_params *params);
// EXEC_CMD//
void		exec_cmd(t_token *token, t_params *params, int i, int *old_fd);
// FT_PIPES//
int			ft_pipe(t_token *token, t_params *params, int *pid,
				t_pipe_fd *pipe_fd);
// IS_BUILTIN//
int			get_path(char **arg, t_token *token, t_params *params, int *old_fd);
void		is_builtin(t_token *token, t_params *params, int i,
				int *old_fd);

////////////INIT///////////////
// INIT_UTILS//
t_export	*init_export(char **env);
void		init_token(t_token *token);
// INIT//
void		init_data(t_data *data);
char		**init_env(char **envp);
int			init_params(t_params *params, char **envp);
int			ft_shlvl(char **envp);
int			wrong_malloc(t_params params);

//////////////PARSING/////////////
// START_ARG//
int			ft_start(t_data *data, t_token *token);
// CMD_SPLIT//
int			split_cmd(t_data *data, t_params *params);
void		replace_quotes(char *str);
// QUOTES_REDIR//
int			quotes_redir(t_token *token, t_params *params);
// IF_DOLL//
int			if_dollar(char **str_value,
				t_data *data, t_params *par, int *quote);
int			expand(t_token *token, t_data *data, t_params *params);
// ONLY_HEREDOCS
void		only_heredocs(t_data *data, t_params *params);
// CHECK_REDIR//
int			check_redir(t_data *data);
// PIPE_SPLIT//
void		jump_quotes(char *str, t_data *data);
int			syntax_check(t_data *data);
int			check_string(t_data *data);
int			pipe_split(t_data *data);
// NO_QUOTES//
int			get_next_quote(char quote, t_data *data);
int			check_quotes(t_data *data);
int			move_to_supp(char *str, int j);
int			no_quotes(t_token *token, t_params *params);
// IS_REDIR//
int			is_redir(t_data *data, t_token *token);

//////////// REDIR///////////////
// FT_HEREDOC_UTILS//
char		*expand_heredoc(char *line, t_params *params, int *i);
void		print_error_heredoc(char *str, int fd, t_params *params);
void		free_heredoc(t_params *params, int fd);			
// FT_DELIM//
int			delim_quotes(char *delim);
// FT_HEREDOCS//
int			ft_heredoc(char *str, t_params *params);
// FT_HEREDOC_UTILS2//
int			get_hd_line(char *del, int fd, int quotes, t_params *params);
// REDIR//
int			fd_in(t_params *params, char *pathname, char *red);
void		ft_redir(char **str, t_params *params, t_token *token);

/////////////UTILS/////////////
// FT_LIST//
int			push_back(t_data *data, char *value);
int			ft_size(t_token *lst);
// FREE//
void		*free_export(t_export *export);
int			free_exit(t_params *params, t_data *data, int *old_fd);
void		free_struct(t_data *data);
void		free_params(t_params *params);
void		free_cmd(t_params *params, int *old_fd);
// PROTECTION//
int			check_fd(t_params *params, int fd, char *red);
int			check_child(t_params *params, int pid);
int			set_err_malloc(char *str);
void		*err_malloc_return(char *str, void *ptr);
void		free_arr(char **table);
// UTILS//
void		replace_quotes2(char *str);
int			ft_space(char c);
void		ft_exit_d(t_data *data, t_params *params);
int			print_error_redir(char *str, char c);
void		print_tab(char **table);

#endif
