CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
BONUS_DIR = bonus
OBJ_DIR = obj
BIN_DIR = bin

$(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

$(OBJ_DIR)/main $(OBJ_DIR)/bonus:
	mkdir -p $@

SRC = $(SRC_DIR)/pipex.c $(SRC_DIR)/utils.c $(SRC_DIR)/split.c
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/main/%.o)

BONUS_SRC = $(BONUS_DIR)/pipex_bonus.c $(BONUS_DIR)/utils_bonus.c $(BONUS_DIR)/split_bonus.c \
$(BONUS_DIR)/get_next_line.c $(BONUS_DIR)/child_process.c $(BONUS_DIR)/here_doc.c \
$(BONUS_DIR)/free_and_pathenv.c
BONUS_OBJ = $(BONUS_SRC:$(BONUS_DIR)/%.c=$(OBJ_DIR)/bonus/%.o)

NAME = $(BIN_DIR)/pipex
BONUS_NAME = $(BIN_DIR)/pipex_bonus

all: $(NAME) $(BONUS_NAME)

man: $(NAME)

bonus: $(BONUS_NAME)

$(NAME): $(OBJ) | $(BIN_DIR)
	$(CC) $(OBJ) -o $(NAME)

$(BONUS_NAME): $(BONUS_OBJ) | $(BIN_DIR)
	$(CC) $(BONUS_OBJ) -o $(BONUS_NAME)

$(OBJ_DIR)/main/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)/main
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/bonus/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR)/bonus
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)

re: fclean all

.PHONY: all clean fclean re
