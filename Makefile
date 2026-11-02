# ---- Project ---------------------------------------------------------------
NAME    = sandbox

SRC_DIR = src
INC_DIR = include
BUILD   = build
OBJ_DIR = $(BUILD)/obj

# Recursive source discovery (src/ and any subdirectories)
SRC     = $(shell find $(SRC_DIR) -name '*.c')
OBJ     = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEP     = $(OBJ:.o=.d)

# ---- Toolchain -------------------------------------------------------------
CC      = gcc
CPPFLAGS = -I$(INC_DIR) -MMD -MP
CFLAGS  = -Wall -Wextra -std=c23 -O2
LDFLAGS =
LDLIBS  =

# `make DEBUG=1` for a debug build
ifeq ($(DEBUG),1)
    CFLAGS = -Wall -Wextra -std=c23 -g -O0 -fsanitize=address,undefined
    LDFLAGS += -fsanitize=address,undefined
endif

# ---- Platform --------------------------------------------------------------
ifeq ($(OS),Windows_NT)
    TARGET = $(BUILD)/$(NAME).exe
    LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
    # `make RELEASE=1` hides the console window
    ifeq ($(RELEASE),1)
        LDFLAGS += -mwindows
    endif
else
    TARGET = $(BUILD)/$(NAME)
    LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# ---- Rules -----------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD)

.PHONY: all run clean

-include $(DEP)
