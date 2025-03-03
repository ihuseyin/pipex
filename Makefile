BOLD = \033[1m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
BLUE = \033[34m
CYAN = \033[36m
RESET = \033[0m

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
BONUS_DIR = bonus
OBJ_DIR = obj
BIN_DIR = bin

SRC = $(SRC_DIR)/pipex.c $(SRC_DIR)/utils.c $(SRC_DIR)/split.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/main/%.o)

BONUS_SRC = $(BONUS_DIR)/pipex_bonus.c $(BONUS_DIR)/utils_bonus.c $(BONUS_DIR)/split_bonus.c \
			$(BONUS_DIR)/get_next_line.c $(BONUS_DIR)/child_process.c $(BONUS_DIR)/here_doc.c \
			$(BONUS_DIR)/free_and_pathenv.c
BONUS_OBJ = $(BONUS_SRC:$(BONUS_DIR)/%.c=$(OBJ_DIR)/bonus/%.o)

NAME = $(BIN_DIR)/pipex
BONUS_NAME = $(BIN_DIR)/bonus/pipex

all: $(NAME) $(BONUS_NAME)

man: $(NAME)

bonus: $(BONUS_NAME)

$(OBJ_DIR) $(BIN_DIR) $(BIN_DIR)/bonus:
	@mkdir -p $@
	@echo "$(CYAN)Creating directories...$(RESET)"

$(OBJ_DIR)/main $(OBJ_DIR)/bonus:
	@mkdir -p $@
	@echo "$(CYAN)Creating main and bonus object directories...$(RESET)"

$(NAME): $(OBJ) | $(BIN_DIR)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	$(CC) $(OBJ) -o $(NAME)
	@echo "$(GREEN)$(BOLD)$(NAME) created successfully!$(RESET)"

$(BONUS_NAME): $(BONUS_OBJ) | $(BIN_DIR)/bonus
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	$(CC) $(BONUS_OBJ) -o $(BONUS_NAME)
	@echo "$(GREEN)$(BOLD)$(NAME) created successfully!$(RESET)"

$(OBJ_DIR)/main/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/main
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/bonus/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR)/bonus
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Cleaned object files...$(RESET)"

fclean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN_DIR)
	@echo "$(RED)Cleaned object files and directories...$(RESET)"

re: fclean all

.PHONY: all man bonus clean fclean re
