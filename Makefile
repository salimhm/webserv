CC			=	c++

CPPFLAGS	=	-Wall -Werror -Wextra

SRCS		=	main.cpp

OBJS		=	$(SRCS:.cpp=.o)

NAME		=	webserv

%.o:		%.cpp *.hpp
			$(CC) $(CPPFLAGS) -c $< -o $@

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(OBJS) $(CPPFLAGS) -o $(NAME)

clean:
			rm -f $(OBJS)

fclean:		clean
			rm $(NAME)

re:			fclean all