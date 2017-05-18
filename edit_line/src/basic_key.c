/*
** basic_key.c for  in /home/januar_m/delivery/PSU/edit_line
** 
** Made by Martin Januario
** Login   <martin.januario@epitech.eu>
** 
** Started on  Wed May 17 17:30:27 2017 Martin Januario
** Last update Wed May 17 18:25:08 2017 Martin Januario
*/

#include	<curses.h>
#include	<term.h>
#include	<stdlib.h>
#include	"edit.h"

void		key_left_(t_key *keys, char *str, char *line)
{
  if (keys->idx < 0)
    return ;
  keys->idx -= 1;
  my_putstr(tgetstr("le", NULL));
  (void) str;
  (void) line;
}

void		key_right_(t_key *keys, char *str, char *line)
{
  if (keys->idx >= my_strlen(line))
    return ;
  keys->idx += 1;
  my_putstr(tgetstr("nd", NULL));
  (void) str;
  (void) line;
}

void		key_top_(t_key *keys, char *str, char *line)
{
  (void) keys;
  (void) str;
  (void) line;
}

void		key_bottom_(t_key *keys, char *str, char *line)
{
  (void) keys;
  (void) str;
  (void) line;
}

