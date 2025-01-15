/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eviala <eviala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:51:45 by eviala            #+#    #+#             */
/*   Updated: 2024/10/17 11:58:43 by eviala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <fcntl.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;
/* ____  _     _ _             _
  / __ \| |   | (_)           | |
 | |  | | |__ | |_  __ _  __ _| |_ ___  _ __ _   _
 | |  | | '_ \| | |/ _` |/ _` | __/ _ \| '__| | | |
 | |__| | |_) | | | (_| | (_| | || (_) | |  | |_| |
  \____/|_.__/|_|_|\__, |\__,_|\__\___/|_|   \__, |
					__/ |                     __/ |
					|___/                     |___/ */

long				ft_strlon(const char *str, char **endptr, int base);
int					ft_tolower(int c);
int					ft_toupper(int c);
int					ft_isalnum(int i);
int					ft_isalpha(int i);
int					ft_isascii(int i);
int					ft_isdigit(int i);
int					ft_isprint(int i);
int					ft_isplusminus(int i);
int					ft_atoi(const char *nptr);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
int					ft_strrchr_str(const char *haystack, const char *needle);
int					ft_count_occ(char *str, char c);
void				ft_free_tab(char **tab);
int					ft_strchr_char_ref(char *str, char *cmp, char **result);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memchr(const void *s, int c, size_t n);
void				*ft_memset(void *b, int c, size_t len);
void				*ft_memcpy(void *dest, const void *src, size_t n);
void				*ft_calloc(size_t nb, size_t size_type);
void				ft_bzero(void *s, size_t n);
void				ft_putnbr_fd(int n, int fd);
void				ft_putchar_fd(char c, int fd);
void				ft_putendl_fd(char *str, int fd);
void				ft_putstr_fd(char *str, int fd);
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
size_t				ft_strlcat(char *dest, const char *src, size_t size);
size_t				ft_strlcpy(char *dest, const char *src, size_t size);
size_t				ft_strlen(const char *str);
char				*ft_itoa(int n);
char				*ft_strchr_char(char *str, char *cmp);
char				*ft_strcpy(char *dst, const char *src);
char				*ft_strchr(const char *s, int c);
char				*ft_strdup(const char *src);
char				*ft_strjoin(const char *s1, const char *s2);
char				*ft_strjoin_free(char *s1, char *s2);
char				*ft_strrchr(const char *s, int c);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strnstr(const char *haystack, const char *needle,
						size_t len);
char				**ft_split(const char *s, char c);
char				*ft_strtrim(char const *s1, char const *set);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_putchar(char c);
void				ft_putstr(const char *str);
void				ft_putnbr(int nb);
/*____
 |  _ \
 | |_) | ___  _ __  _   _ ___
 |  _ < / _ \| '_ \| | | / __|
 | |_) | (_) | | | | |_| \__ \
 |____/ \___/|_| |_|\__,_|___/
*/

void				ft_lstadd_back(t_list **alst, t_list *lst_new);
void				ft_lstadd_front(t_list **alst, t_list *lst_new);
void				ft_free_lst(t_list **lst, void (*del)(void *));
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
void				ft_lstdisplay(t_list *liste);
t_list				*ft_lstlast(t_list *lst);
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));
t_list				*ft_lstnew(void *content);
int					ft_lstsize(t_list *lst);

/*   _____ _   _ _
  / ____| \ | | |
 | |  __|  \| | |
 | | |_ | . ` | |
 | |__| | |\  | |____
  \_____|_| \_|______|

						*/

char				*get_next_line(int fd);
/*  _____      _       _    __
	|  __ \    (_)     | |  / _|
	| |__) | __ _ _ __ | |_| |_
	|  ___/ '__| | '_ \| __|  _|
	| |   | |  | | | | | |_| |
	|_|   |_|  |_|_| |_|\__|_|
*/

int					ft_putchar_fd_printf(char c, int fd);
int					ft_printf(int fd, const char *str, ...);
int					ft_putstr_fd_printf(char *str, int fd);
int					ft_putnbr_fd_printf(int n, int fd);
int					ft_putunbr_fd_printf(unsigned int n, int fd);
int					ft_putptr_fd_printf(unsigned long n, int fd);
int					ft_puthex_fd_printf(unsigned int n, char format, int fd);

#endif
