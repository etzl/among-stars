###### VARIABLES #############
# Directories
INCDIR := include
SRCDIR := src
OBJDIR := src/obj

# Compiler and options
CC := g++
FLAGS := -lpanel -lmenu -lncurses -lpthread -I$(INCDIR) -std=c++17 -g -ftrapv -Woverflow -Wconversion -Wpedantic -Wextra -Wall -Wdouble-promotion -Wformat=2

# Target executable
TARGET := among_stars
#############################

# Needed files
DEP := $(shell find $(INCDIR) -type f -name *.h)
SRC := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJ := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC:.cpp=.o))

# Non-file targets
.PHONY: all dir remove clean run

all: dir $(TARGET)

# Object-files rule
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEP)
	$(CC) -c -o $@ $< $(FLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(FLAGS)
	@echo "\nBUILD SUCCESSFUL. start the program using 'make run' or ./$(TARGET)"

remove:
	-@rm $(TARGET)

clean: remove
	-@rm -rf $(OBJDIR)/*.o
	@echo "Cleared executable and object files!"

dir:
	@mkdir -p $(OBJDIR)

run:
	@./$(TARGET)
