SRCS		=	main.cpp
OBJS		= ${SRCS:.cpp=.o}
CPPC		= clang++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98
NAME		= a.out
RM			= rm -f

%.o :		%.cpp
			${CPPC} ${CPPFLAGS} -c $<
all :		${NAME}
${NAME} :	${OBJS}
			${CPPC} -o ${NAME} ${OBJS}
clean :
			${RM} ${OBJS}
fclean :	clean
			${RM} *.out
re :		fclean all
.PHONY :
			all ${NAME} clean fclean re
