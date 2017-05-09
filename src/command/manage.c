/*
** manage.c for minishell2 in /home/sahel/rendu/PSU/PSU_2016_minishell2
**
** Made by Sahel Lucas--Saoudi
** Login   <sahel.lucas-saoudi@epitech.eu>
**
** Started on  Wed Apr  5 17:04:35 2017 Sahel Lucas--Saoudi
** Last update Fri Apr 28 17:59:37 2017 Sahel Lucas--Saoudi
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "main.h"
#include "basic.h"

static void		exec_error(char **argv, t_shell *shell)
{
  struct stat		st_file;

  shell->status = 1;
  if ((access(argv[0], 0) == 0 || (lstat(argv[0], &st_file) == 0 &&
				   S_ISLNK(st_file.st_mode))))
    {
      putstr_(argv[0], 2);
      putstr_(": Permission denied.\n", 2);
    }
  else
    {
      putstr_(argv[0], 2);
      putstr_(": Command not found.\n", 2);
    }
}

void		exec(char **argv, t_shell *shell)
{
  int		pid;

  pid = fork();
  if (pid == 0)
    {
      execve(argv[0], argv, shell->env);
      putstr_(argv[0], 2);
      putstr_(": Exec format error. Binary file not executable.\n", 2);
      exit(1);
    }
  else
    {
      waitpid(pid, &shell->status, 0);
      error_handling(shell->status);
      shell->status %= 255;
      if (shell->status <= 30 && shell->status > 3)
	shell->status += 128;
    }
}

void		part2_exec(char **argv, t_shell *shell)
{
  struct stat	st;

  if (sp_len_(argv[0], '/') != strlen_(argv[0])){
      if (lstat(argv[0], &st) == 0 && access(argv[0], X_OK) == 0 &&
	  !S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode))
	exec(argv, shell);
      else
	exec_error(argv, shell);
    }
  else if (set_path(argv, shell) == 1)
    exec_error(argv, shell);
  else
    exec(argv, shell);
}

void		exec_manager(char **argv, t_shell *shell)
{
  static char	**dico;
  int		i;
  int		(*func[5])(char **, t_shell *) =
    {call_cd, call_setenv, call_unsetenv, call_env, call_exit};

  i = 0;
  if (!dico)
    dico = c_dico(5, "cd", "setenv", "unsetenv", "env", "exit");
  if (!argv || !*argv)
    return ;
  while (dico[i])
    {
      if (match(argv[0], dico[i]))
	{
	  shell->status = func[i](argv, shell);
	  return ;
	}
      i++;
    }
  part2_exec(argv, shell);
}

void	manage_command(t_shell *shell)
{
  char	**argv;
  char	*tmp;

  if (howmany_(shell->command, '\"') % 2 != 0)
    {
      putstr_("Unmatched '\"'.\n", 2);
      shell->status = 1;
      return ;
    }
  tmp = epur(shell->command);
  free_(shell->command);
  shell->command = tmp;
  if (!shell->command[0])
    return ;
  argv = parse_(shell->command, ';');
  if (!argv[0] && howmany_(shell->command, '\"') != 0)
    putstr_(": Command not found.\n", 2);
  if (validity(argv) || (!argv[0] && howmany_(shell->command, '\"') != 0))
    {
      shell->status = 1;
      free_tab(argv);
      return ;
    }
  lexer(argv, shell);
  free_tab(argv);
}
