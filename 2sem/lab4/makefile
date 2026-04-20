CC = gcc
FLAG = -Werror
DEBUG_FLAGS = -g -O0

OBJ_DIR = obj
DEBUG_OBJ_DIR = obj_debug
SOURCES = avl_tree.c main.c
TEST_SOURCES = avl_tree.c test_avl.c

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:.c=.o))
DEBUG_OBJECTS = $(addprefix $(DEBUG_OBJ_DIR)/, $(SOURCES:.c=_debug.o))

TARGET = main.exe
DEBUG_TARGET = main_debug.exe
TEST_TARGET = test_avl.exe

all: build

$(OBJ_DIR):
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(DEBUG_OBJ_DIR):
	if not exist $(DEBUG_OBJ_DIR) mkdir $(DEBUG_OBJ_DIR)

build: $(TARGET)

$(TARGET): $(OBJ_DIR) $(OBJECTS)
	$(CC) $(FLAG) $(OBJECTS) -o $(TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJ_DIR) $(DEBUG_OBJECTS)
	$(CC) $^ -o $@ $(DEBUG_FLAGS)

test: $(TEST_TARGET)
	.\$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CC) $(FLAG) $(TEST_SOURCES) -o $(TEST_TARGET)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(FLAG) -c $^ -o $@

$(DEBUG_OBJ_DIR)/%_debug.o: %.c | $(DEBUG_OBJ_DIR)
	$(CC) $(DEBUG_FLAGS) -c $^ -o $@

run: build
	.\$(TARGET) $(ARGS)

rundbg: $(DEBUG_TARGET)
	gdb -tui .\$(DEBUG_TARGET)

clean:
	-del /Q $(OBJECTS) $(DEBUG_OBJECTS) $(TARGET) $(DEBUG_TARGET) $(TEST_TARGET) 2>nul
