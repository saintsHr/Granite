# ====================================================
# Variables
# ====================================================
CC  := gcc
CXX := g++
AR  := ar
RM  := rm -rf

TARGET := granite
LIB := lib/lib$(TARGET).a

SRC_DIR   := src
BUILD_DIR := build
LIB_DIR   := lib
INC_DIR   := include

INCLUDE := -I$(INC_DIR)

# ====================================================
# Sources
# ====================================================
CPP_SRC := $(shell find $(SRC_DIR) -name "*.cpp")
C_SRC   := $(shell find $(SRC_DIR) -name "*.c")

OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRC)) \
       $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRC))

DEP := $(OBJ:.o=.d)

# ====================================================
# Flags
# ====================================================
DEPFLAGS := -MMD -MP

CFLAGS   := -Wall -Wextra -std=c11   $(INCLUDE) $(DEPFLAGS)
CXXFLAGS := -Wall -Wextra -std=c++20 $(INCLUDE) $(DEPFLAGS)

RELEASE_CFLAGS   := -O3 $(CFLAGS)
RELEASE_CXXFLAGS := -O3 $(CXXFLAGS)

# ====================================================
# Main Rules
# ====================================================
.PHONY: all release clean

all: $(LIB)

release: CFLAGS   := $(RELEASE_CFLAGS)
release: CXXFLAGS := $(RELEASE_CXXFLAGS)
release: $(LIB)

# ====================================================
# Library
# ====================================================
$(LIB): $(OBJ)
	@mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $(OBJ)

# ====================================================
# Object Compiling
# ====================================================
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ====================================================
# cloc (count lines of code)
# ====================================================
cloc:
	cloc ./src ./include --exclude-list-file=clocignore.txt

# ====================================================
# Automatic Dependencies
# ====================================================
-include $(DEP)

# ====================================================
# Cleaning
# ====================================================
clean:
	$(RM) $(BUILD_DIR) $(LIB_DIR)