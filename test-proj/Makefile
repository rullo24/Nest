CC = gcc
CFLAGS = -Wall -I./code $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = -lUser32 $(shell pkg-config --libs gtk+-3.0)

SRC_DIR = ./
OBJ_DIR = ./Objects

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
EXECUTABLE = Test.exe

all: $(OBJ_DIR) $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR):
	@if not exist $(OBJ_DIR) mkdir $(subst /,\,$(OBJ_DIR))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	@if exist $(OBJ_DIR) rmdir /s /q $(subst /,\,$(OBJ_DIR))
	@if exist $(EXECUTABLE) del $(subst /,\,$(EXECUTABLE))
