# Defining the compiler to be used and all necessary flags
CC = gcc
CFLAGS = -Wall -I./code $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -lUser32 $(shell pkg-config --libs gtk+-3.0)
WINDOWS_FLAGS = # -mwindows 	<-- uncomment this to remove the windows terminal link to the application

# Specifying the location of the source files and where to store the produced object files
SRC_DIR = ./code/src
OBJ_DIR = ./code/Objects

# Collecting source files and generating all object files
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
EXECUTABLE = Nest.exe

# Rule set to build all
all: $(OBJ_DIR) $(EXECUTABLE)

# Rule to link the object files into the .exe
$(EXECUTABLE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) $(WINDOWS_FLAGS)

# Rule to create the obj dir if it DNE
$(OBJ_DIR):
	@if not exist $(OBJ_DIR) mkdir $(subst /,\,$(OBJ_DIR))

# Rule to compile each .c file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# Rule to remove the .exe and object files when "make clean" is run
clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(subst /,\,$(OBJ_DIR))
	@if exist $(EXECUTABLE) del $(subst /,\,$(EXECUTABLE))