NAME		=	computor_v1

M_TASK		=	mandatory_task
B_TASK		=	bonus_task

OBJ_DIR		=	obj
RM_DIR		=	rm -rf
RM_FILE		=	rm -f

#COLORS
C_NO		=	"\033[00m"
C_OK		=	"\033[32m"
C_GOOD		=	"\033[32m"

#DEBUG
SUCCESS		=	$(C_GOOD)SUCCESS$(C_NO)
OK			=	$(C_OK)OK$(C_NO)

all			:
				@make -C ${M_TASK}
				cp ${M_TASK}/${NAME} ${NAME}

bonus		:
				@make -C ${B_TASK}
				cp ${B_TASK}/${NAME} ${NAME}

clean		:
				@$(RM_DIR) $(OBJ_DIR)
				@echo "\tCleaning...\t" [ $(NAME) ] $(OK)

fclean		:	clean
				@$(RM_FILE) $(M_TASK)/$(NAME)
				@$(RM_FILE) $(B_TASK)/$(NAME)
				@$(RM_FILE) $(NAME)
				@echo "\tDeleting...\t" [ $(NAME) ] $(OK)

re			:	fclean all

.PHONY		:	all, bonus, clean, fclean, re