/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gmorer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 18:17:49 by gmorer            #+#    #+#             */
/*   Updated: 2019/08/10 11:51:40 by gmorer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/stat.h>

void				*ft_memcpy(void *dst, const void *src, size_t n);
char				*ft_strcpy(char *dst, const char *src);
void				ft_bzero(void *s, size_t n);
size_t				ft_strlen(const char *str);
void				*ft_memset(void *b, int c, size_t len);
char				*ft_strchr(const char *s, int c);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strdup(const char *s1);
size_t				ft_strlen(const char *s);
char				*ft_strnstr(const char *s1, const char *s2, size_t n);
void				*ft_memmove(void *dst, const void *src, size_t len);

#endif
