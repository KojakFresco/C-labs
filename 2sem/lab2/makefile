CC = gcc
FLAG = -Werror
DEBUG_FLAGS = -g -O0

OBJ_DIR = obj
DEBUG_OBJ_DIR = obj_debug
SOURCES = sort.c main.c

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
DEBUG_OBJECTS = $(addprefix $(DEBUG_OBJ_DIR)/, $(SOURCES:.c=_debug.o))

TARGET = main.exe
DEBUG_TARGET = main_debug.exe

all: run

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(DEBUG_OBJ_DIR):
	mkdir $(DEBUG_OBJ_DIR)

build: $(OBJ_DIR) $(OBJECTS)
	$(CC) $(FLAG) $(OBJECTS) -o main

$(DEBUG_TARGET): $(DEBUG_OBJ_DIR) $(DEBUG_OBJECTS)
	$(CC) $^ -o $@ $(DEBUG_FLAGS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(FLAG) -c $^ -o $@

$(DEBUG_OBJ_DIR)/%_debug.o: %.c
	$(CC) $(DEBUG_FLAGS) -c $^ -o $@

run: build
	main

rundbg: $(DEBUG_TARGET)
	gdb -tui ./$(DEBUG_TARGET)

clean:
	del /Q $(OBJECTS) $(DEBUG_OBJECTS) 2>nul || true
