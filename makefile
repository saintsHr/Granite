# ====================================================
# Compiler Setup
# ====================================================
CC      ?= gcc
CXX     ?= g++
AR      := ar
RM      := rm -rf

COMPILER_NAME := $(notdir $(CXX))

IS_CLANG := $(filter clang++,$(COMPILER_NAME))
IS_GCC   := $(filter g++,$(COMPILER_NAME))

# ====================================================
# Project Structure
# ====================================================
TARGET       := granite
LIB          := lib/lib$(TARGET).a

SRC_DIR      := src

BUILD_TYPE 	   ?= debug
BUILD_BASE_DIR  = build
BUILD_DIR  	    = $(BUILD_BASE_DIR)/$(BUILD_TYPE)

LIB_DIR      := lib
INC_DIR      := include
THIRD_PARTY  := third-party
THIRD_SRC    := $(THIRD_PARTY)/src

INCLUDE := -I$(INC_DIR) -I$(THIRD_PARTY)/include
DEFS := -DGLFW_INCLUDE_NONE

DEPFLAGS := -MMD -MP

# ====================================================
# Sources
# ====================================================
CPP_ALL := $(shell find $(SRC_DIR) -type f -name '*.cpp')
C_ALL   := $(shell find $(SRC_DIR) -type f -name '*.c')

CPP_EXTERNAL := $(shell find $(THIRD_SRC) -type f -name '*.cpp')
C_EXTERNAL   := $(shell find $(THIRD_SRC) -type f -name '*.c')

OBJ_GRANITE = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_ALL))
OBJ_GRANITE += $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_ALL))

OBJ_EXTERNAL = $(patsubst $(THIRD_SRC)/%.cpp,$(BUILD_DIR)/third-party/src/%.o,$(CPP_EXTERNAL))
OBJ_EXTERNAL += $(patsubst $(THIRD_SRC)/%.c,$(BUILD_DIR)/third-party/src/%.o,$(C_EXTERNAL))

OBJ_CURRENT = $(OBJ_GRANITE) $(OBJ_EXTERNAL)

# ====================================================
# Base Flags
# ====================================================
CFLAGS   := -Wall -Wextra -std=c17 $(DEFS) $(INCLUDE) $(DEPFLAGS)
CXXFLAGS := -Wall -Wextra -std=c++20 $(DEFS) $(INCLUDE) $(DEPFLAGS)

RELEASE_CFLAGS   := -O3 -DNDEBUG $(CFLAGS)
RELEASE_CXXFLAGS := -O3 -DNDEBUG $(CXXFLAGS)

DEBUG_CFLAGS := \
-O0 \
-g3 \
-fdiagnostics-color=always \
-fdiagnostics-show-option \
-fdiagnostics-show-caret \
-fmessage-length=0 \
-ftemplate-backtrace-limit=0 \
-fno-omit-frame-pointer \
-D_GLIBCXX_ASSERTIONS \
$(CFLAGS)

DEBUG_CXXFLAGS := \
-O0 \
-g3 \
-fdiagnostics-color=always \
-fdiagnostics-show-option \
-fdiagnostics-show-caret \
-fmessage-length=0 \
-ftemplate-backtrace-limit=0 \
-fno-omit-frame-pointer \
-D_GLIBCXX_ASSERTIONS \
$(CXXFLAGS)

# ====================================================
# CI Strict Flags
# ====================================================
CI_BASE := \
	-Wall -Wextra -Wpedantic -Werror \
	-Wconversion -Wsign-conversion -Wshadow \
	-Wcast-align -Wdouble-promotion \
	-Wformat=2 -Wundef -Wswitch-enum -Wswitch-default \
	-Wimplicit-fallthrough \
	-fsanitize=address,undefined \
	-fno-omit-frame-pointer -g3

# GCC-specific warnings
ifeq ($(IS_GCC),g++)
CI_BASE += \
	-Wduplicated-cond \
	-Wduplicated-branches \
	-Wlogical-op \
	-Wuseless-cast \
	-Wnull-dereference
endif

# Clang-specific flags
ifeq ($(IS_CLANG),clang++)
CI_BASE += \
	-fsanitize-address-use-after-scope
endif

CI_C_EXTRA := \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wmissing-declarations

CI_CXX_EXTRA := \
	-Wnon-virtual-dtor \
	-Wold-style-cast \
	-Woverloaded-virtual \
	-Wzero-as-null-pointer-constant \
	-Wextra-semi \
	-Wredundant-decls \
	-Wsign-promo

# ====================================================
# Targets
# ====================================================
.PHONY: all release ci ci-release clean cloc

all: $(LIB)

release:
	$(MAKE) BUILD_TYPE=release

ci:
	$(MAKE) BUILD_TYPE=ci

ci-release:
	$(MAKE) BUILD_TYPE=ci-release

ifeq ($(BUILD_TYPE),release)
CFLAGS   := $(RELEASE_CFLAGS)
CXXFLAGS := $(RELEASE_CXXFLAGS)
endif

ifeq ($(BUILD_TYPE),debug)
CFLAGS   := $(DEBUG_CFLAGS)
CXXFLAGS := $(DEBUG_CXXFLAGS)
endif

ifeq ($(BUILD_TYPE),ci)
CFLAGS   := $(CI_BASE) $(CI_C_EXTRA) -std=c17 $(DEFS) $(INCLUDE) $(DEPFLAGS)
CXXFLAGS := $(CI_BASE) $(CI_CXX_EXTRA) -std=c++20 $(DEFS) $(INCLUDE) $(DEPFLAGS)
endif

# ====================================================
# Library
# ====================================================
$(LIB): $(OBJ_CURRENT)
	@mkdir -p $(LIB_DIR)
	$(AR) rcs $@ $(OBJ_CURRENT)

# ====================================================
# Build Rules
# ====================================================
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# External sources (minimal warnings)
$(BUILD_DIR)/third-party/src/%.o: $(THIRD_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) -std=c17 -I$(THIRD_PARTY)/include -c $< -o $@

$(BUILD_DIR)/third-party/src/%.o: $(THIRD_SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -std=c++20 -I$(THIRD_PARTY)/include -c $< -o $@

# ====================================================
# cloc
# ====================================================
cloc:
	cloc ./src ./include --exclude-list-file=clocignore.txt

# ====================================================
# Dependencies
# ====================================================
-include $(OBJ_CURRENT:.o=.d)

# ====================================================
# Clean
# ====================================================
clean:
	$(RM) $(BUILD_BASE_DIR) $(LIB_DIR)