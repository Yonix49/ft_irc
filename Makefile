# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhajji-b <mhajji-b@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/20 13:39:44 by kgezgin           #+#    #+#              #
#    Updated: 2023/11/10 12:54:46 by mhajji-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 
CXXFLAGS += -MMD
INCLUDE = -I. 

SRCDIR = srcs
OBJDIR = obj

# List of source files
SRCS = $(wildcard $(SRCDIR)/*.cpp) \
       $(wildcard $(SRCDIR)/channel/*.cpp) \
       $(wildcard $(SRCDIR)/server/*.cpp) \
       $(wildcard $(SRCDIR)/server/cmd/*.cpp) \
       $(wildcard $(SRCDIR)/user/*.cpp) \
       $(wildcard $(SRCDIR)/utils/*.cpp)

# Generate object file names from source file names
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

NAME = span

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CPPFLAGS) $(INCLUDE) -o $(NAME) $(OBJS)

-include $(DEPS)

# Rule for creating object files in the corresponding subdirectories
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

leak: $(NAME)
	make && valgrind --leak-check=full --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re leak

