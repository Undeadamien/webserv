# Nom du programme
NAME = webserv

# Dossier de sortie pour les fichiers objets
OBJ_DIR = obj/
SRCS_DIR = srcs/

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Headers et includes
HEADER = -I./includes \
		-I./libft/includes

# Options de compilation
CPPFLAGS = -Wall -Wextra -Werror -std=c++98

# Commandes
CPP = c++
RM = rm -rf

# Rechercher tous les fichiers .cpp dans le répertoire courant
SRCS = $(shell find $(SRCS_DIR) -type f -name "*.cpp" | sed "s|^$(SRCS_DIR)||")

MAKEFLAGS += --no-print-directory
TOTAL_FILES := $(words $(SRCS))
CURRENT_INDEX := 0

# Conversion des fichiers source en fichiers objets
OBJS = $(addprefix ${OBJ_DIR}, $(SRCS:.cpp=.o))

# Cibles
all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)
	@echo "\n"

# Compilation du programme
$(NAME): $(LIBFT) $(OBJS)
	@$(CPP) $(CPPFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(HEADER)
	@echo "\n┗▷${BOLD}$(GREEN)『./$(NAME) Successfully created [✅]$(RESET)"

${OBJ_DIR}%.o: ${SRCS_DIR}%.cpp
	@mkdir -p $(dir $@)
	@$(eval CURRENT_INDEX=$(shell echo $$(($(CURRENT_INDEX)+1))))
	@$(eval PERCENT=$(shell echo $$(($(CURRENT_INDEX) * 100 / $(TOTAL_FILES)))))
	@printf "\r🔧 $(BLUE)$(BOLD)%3d%% $(WHITE)$(BOLD)Compiling:$(WHITE)$(BOLD) ${NAME}${RESET} $(RED)$(BOLD)%-50s $(MAGENTA)[%3d/%3d]$(RESET)" $(PERCENT) "$<" $(CURRENT_INDEX) $(TOTAL_FILES)
	@${CPP} ${CPPFLAGS} ${HEADER} -c $< -o $@

# Nettoyage des fichiers objets
clean:
	@$(MAKE) clean -C $(LIBFT_DIR) > /dev/null 2>&1
	@echo "$(BOLD) [ 🗑️ ] $(YELLOW)$(REVERSED)Cleaning up$(RESET)"
	@$(RM) -r $(OBJ_DIR)
	@echo "┗▷$(YELLOW)『$(ITALIC)./$(SRCS_DIR)'s$(RESET)$(YELLOW) object files cleaned』$(RESET)"

# Nettoyage complet
fclean: clean
	@$(MAKE) fclean -C $(LIBFT_DIR) > /dev/null 2>&1
	@$(RM) $(NAME)
	@echo "┗▷$(YELLOW)『$(ITALIC)./$(NAME)'s$(RESET)$(YELLOW) cleaned』$(RESET)"

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
