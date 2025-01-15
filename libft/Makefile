# Nom du programme
NAME = libft.a

# Dossier de sortie pour les fichiers objets
OBJ_DIR = obj/
SRCS_DIR = srcs/

# Headers et includes
HEADER = -I./includes

# Options de compilation
CFLAGS = -Wall -Wextra -Werror

# Commandes
CC = cc
AR = ar rcs
RM = rm -rf

# Rechercher tous les fichiers .c dans le répertoire courant
SRCS		= ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_toupper.c ft_tolower.c ft_strlcpy.c ft_strlcat.c ft_strncmp.c \
	ft_atoi.c ft_strchr.c ft_strrchr.c ft_strnstr.c ft_memcmp.c ft_memchr.c ft_memset.c \
	ft_bzero.c ft_memcpy.c ft_memmove.c ft_strdup.c ft_calloc.c ft_strjoin.c ft_split.c \
	ft_itoa.c ft_substr.c ft_strtrim.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
	ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putchar.c ft_putstr.c\
	ft_putnbr.c ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
	ft_lstadd_back.c ft_lstdelone.c ft_free_lst.c ft_lstiter.c \
	ft_lstmap.c ft_strjoin_free.c ft_isplusminus.c ft_strlon.c \
	get_next_line.c ft_strcpy.c ft_count_occ.c \
	ft_putchar_fd_printf.c ft_printf.c ft_puthex_fd_printf.c ft_putnbr_fd_printf.c \
	ft_putptr_fd_printf.c ft_putstr_fd_printf.c ft_putunbr_fd_printf.c ft_free.c


# Conversion des fichiers source en fichiers objets
OBJS = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.c=.o)))

TOTAL_FILES		:= $(words $(SRCS))
CURRENT_INDEX		:= 0

# Cibles
all: $(NAME)

# Compilation de la bibliothèque
$(NAME): $(OBJS)
	@$(AR) $(NAME) $(OBJS)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c | $(OBJ_DIR)
	@$(eval CURRENT_INDEX=$(shell echo $$(($(CURRENT_INDEX)+1))))
	@$(eval PERCENT=$(shell echo $$(($(CURRENT_INDEX) * 100 / $(TOTAL_FILES)))))
	@printf "\r🔧 $(BLUE)$(BOLD)%3d%% $(WHITE)$(BOLD)Compiling:$(WHITE)$(BOLD) ${NAME}${RESET} $(RED)$(BOLD)%-50s $(MAGENTA)[%3d/%3d]$(RESET)" $(PERCENT) "$<" $(CURRENT_INDEX) $(TOTAL_FILES)
	@$(CC) $(CFLAGS) $(HEADER) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Nettoyage des fichiers objets
clean:
	@echo "$(BOLD) [ 🗑️ ] $(YELLOW)$(REVERSED)Cleaning up$(RESET)"
	@$(RM) -r $(OBJ_DIR) > /dev/null 2>&1
	@echo "┗▷$(YELLOW)『$(ITALIC)./$(SRCS_DIR)'s$(RESET)$(YELLOW) object files cleaned』$(RESET)"

# Nettoyage complet
fclean: clean
	@$(RM) $(NAME) > /dev/null 2>&1
	@echo "┗▷$(YELLOW)『$(ITALIC)./$(NAME)'s$(RESET)$(YELLOW) executable cleaned』$(RESET)"

# Recompiler entièrement
re: fclean all

.PHONY: all clean fclean re

# Colors
RED := \033[0;31m
GREEN := \033[0;32m
YELLOW := \033[0;33m
BLUE := \033[0;34m
MAGENTA := \033[0;35m
CYAN := \033[0;36m
WHITE := \033[0;37m
RESET := \033[0m
BOLD := \033[1m
UNDERLINE := \033[4m
REVERSED := \033[7m
ITALIC := \033[3m
