APP = $(APP_NAME)

APP_NAME=ft_ping #create variable called APP_NAME and assign value to it which is ft_ping

# sources
SRCS_DIR = src

MAIN_DIR = $(SRCS_DIR)
SRCS_MAIN = $(MAIN_DIR)/main.c

PACKET_DIR=$(SRCS_DIR)/packet
SRCS_PACKET = $(addprefix $(PACKET_DIR)/, fill_ping_packet_data.c)
HEADERS_PACKET= $(addprefix $(PACKET_DIR)/, ip_icmp.h packet.h)

IP_PACKET_DIR=$(SRCS_DIR)/ip_packet
SRCS_IP_PACKET = $(addprefix $(IP_PACKET_DIR)/, ip_packet.c)
HEADERS_IP_PACKET = $(addprefix $(IP_PACKET_DIR)/, ip_packet.h) 

TIME_DIR=$(SRCS_DIR)/time
SRCS_TIME=$(addprefix $(TIME_DIR)/, time.c)
HEADERS_TIME=$(addprefix $(TIME_DIR)/, time.h)
# lib

LIBFT_DIR = $(SRCS_DIR)/libft
LIBFT_FILE = libft.a
LIBFT_NAME_SHORT=ft

PING_DATA_DIR = $(SRCS_DIR)/ping_data
HEADERS_PING_DATA = $(addprefix $(PING_DATA_DIR)/, ping_data.h)

ARGS_DIR = $(SRCS_DIR)/args
SRCS_ARGS = $(addprefix $(ARGS_DIR)/, args.c)
HEADERS_ARGS = $(addprefix $(ARGS_DIR)/, args.h)

IPV4_STRING_FORMATTER_DIR = $(SRCS_DIR)/ipv4_string_formatter
SRCS_IPV4_STRING_FOMRATTER = $(addprefix $(IPV4_STRING_FORMATTER_DIR)/, ipv4_string_formatter.c)
HEADERS_IPV4_STRING_FOMRATTER = $(addprefix $(IPV4_STRING_FORMATTER_DIR)/, ipv4_string_formatter.h)

SRCS = $(SRCS_MAIN) $(SRCS_PACKET) $(SRCS_IP_PACKET) $(SRCS_TIME) $(SRCS_ARGS) $(SRCS_IPV4_STRING_FOMRATTER)

HEADERS = $(HEADERS_PACKET) $(HEADERS_IP_PACKET) $(HEADERS_TIME) $(HEADERS_PING_DATA) $(HEADERS_ARGS) $(HEADERS_IPV4_STRING_FOMRATTER)

INCLUDE_DIRS = $(addprefix -I, $(PACKET_DIR) $(LIBFT_DIR) $(IP_PACKET_DIR) $(TIME_DIR) $(PING_DATA_DIR) $(ARGS_DIR) $(IPV4_STRING_FORMATTER_DIR))
# -I followed by dir name - it is dirrectory where compiler will find *.h files
# during compilation process

# objs

OBJS = $(SRCS:.c=.o)

# flags

CC = gcc # Flag for implicit rules

# gcc - is compiler gcc hello.c is to compile file
# -Wall -Wextra -Werror
CFLAGS =  -c -g # Flag for implicit rules.
# -Wall - show all error messager
# -c compile (produce *.o files but not link)
# -g set symbolic points in file, so debugging is possible

# -L followed by path is where libraries can be found
# -l followed by name of library is we specify name of library compiler needs
# library name in Unix is libxxxxx.a 
# these two flags are needed by linker

.PHONY: all clean fclean re run compile_libft #these targets will not be treated as targets that produce files. Target that produce file gets executed if file doesn't exist or if file exists but it its dependecies are fresher. Phone targets gets executed every time they are called
#below is rule. all - rule name, after ":" go dependency that is needed to execute rule
#all is the rule common to all make files. By convention it must compile all system
all: compile_libft $(APP)
	

$(APP): $(OBJS) #here this OBJS variable is expaned to list of .o files. And make will go and look for rule that corresponds to name.o file. And it will find this rule in %.o : %.c
	@ echo target $(APP_NAME)
	@ $(CC) -o $(APP) $(OBJS) -L$(LIBFT_DIR) -l$(LIBFT_NAME_SHORT)
# -o is compiler flag to produce object *.o file with particular name 

#below is % is a vild card. We say that if we want to produce %.o (whatever name dot o) then we need as dependency same name dot c
#$@ this is local variable $@ means take whatever it is on left side of colon in %.o : %.c
# @ echo target means that command itself will not be printed to console
%.o: %.c $(HEADERS)
	@ echo target $@
	$(CC) $(CFLAGS) $(INCLUDE_DIRS) $< -o $@ 

compile_libft:
	@ echo target $@
	@ make -C $(LIBFT_DIR)

#clean is also convetional rule, it must get rid of all built artifacts in the system
clean: clean_libft
	@ echo remove object files
	@ rm -f $(OBJS)

fclean:
	@ echo remove program file
	@ rm -f $(APP_NAME)

clean_libft:
	make clean -C $(LIBFT_DIR)

re: clean fclean all

run: re
	./ft_ping $(MAKE_ARG)

# %.o : %.c
# %.o - target that corresponds to every .o file.
# %.c - converts .o target to .c file name aa.o -> aa.c so we get aa.c prerequisit

#  $@ automatic variable that contains the target name.
#  $< automatic variable that contains the prerequisit name.

