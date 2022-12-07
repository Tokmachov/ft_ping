NAME = ft_ping

SRCS_DIR = src

MAIN_DIR = .

MODULE1_DIR = module1

MODULE2_DIR = module2

SRCS_MAIN = $(SRCS_DIR)/$(MAIN_DIR)/main.c

SRCS_MODULE1 = $(addprefix $(SRCS_DIR)/$(MODULE1_DIR)/, \
	do_something1.c)

SRCS_MODULE2 = $(addprefix $(SRCS_DIR)/$(MODULE2_DIR)/, \
	do_something2.c)

SRCS = $(SRCS_MAIN) $(SRCS_MODULE1) $(SRCS_MODULE2)

HEARES_MODULE1 = $(addprefix $(SRCS_DIR)/$(MODULE1_DIR)/, \
	do_something1.h)

HEARES_MODULE2 = $(addprefix $(SRCS_DIR)/$(MODULE2_DIR)/, \
	do_something2.h)

HEADERS = $(HEARES_MODULE1) $(HEARES_MODULE2)

OBJS = $(SRCS:.c=.o)

CC = gcc # Flag for implicit rules

CFLAGS = -Wall -Wextra -Werror -c -g # Flag for implicit rules. Turn on debug info

INCLUDE_DIRS = $(addprefix -I$(SRCS_DIR)/, \
	$(MODULE1_DIR) $(MODULE2_DIR))


.PHONY: all clean fclean re

all: $(NAME) clean_objs

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

%.o : %.c $(HEADERS)
	@ echo compile $< to $@
	@ $(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o $@ 

clean: clean_objs fclean
	
clean_objs:
	@ echo remove object files
	@ rm -f $(OBJS)
fclean:
	@ echo remove program file
	@ rm -f $(NAME)

re: fclean all

# %.o : %.c
# %.o - target that corresponds to every .o file.
# %.c - converts .o target to .c file name aa.o -> aa.c so we get aa.c prerequisit

#  $@ automatic variable that contains the target name.
#  $< automatic variable that contains the prerequisit name.

