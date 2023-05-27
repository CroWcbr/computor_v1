NAME		=	computor_v1

CC			=	c++ -std=c++17
CFLAGS		=	-Wall -Werror -Wextra

INC_DIR		=	./inc
SRC_DIR		=	./src
OBJ_DIR		=	../obj/bonus

SRC			=	main.cpp \
				Computor_v1.cpp

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

RM_DIR		=	rm -rf
RM_FILE		=	rm -f

#COLORS
C_NO		=	"\033[00m"
C_OK		=	"\033[32m"
C_GOOD		=	"\033[32m"

#DEBUG
SUCCESS		=	$(C_GOOD)SUCCESS$(C_NO)
OK			=	$(C_OK)OK$(C_NO)

$(NAME)		:	$(OBJ) ${INC_DIR}/*.hpp
				$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
				@echo "\tCompiling...\t" [ $(NAME) ] $(SUCCESS)

$(OBJ)		: 	| $(OBJ_DIR)	

$(OBJ_DIR)	:
				@mkdir -p $(OBJ_DIR)
			
$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.cpp ${INC_DIR}/*.hpp Makefile
					$(CC) $(CFLAGS) -c $< -o $@

all			:	$(NAME)

clean		:
				@$(RM_DIR) $(OBJ_DIR)
				@echo "\tCleaning...\t" [ $(OBJ_DIR) ] $(OK)

fclean		:	clean
				@$(RM_FILE) $(NAME)
				@echo "\tDeleting...\t" [ $(NAME) ] $(OK)

re			:	fclean all

.PHONY		:	all, clean, fclean, re
