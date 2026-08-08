.DELETE_ON_ERROR:


#==============CONFIG VARS================
SRC_DIR			?= ../src
OBJ_DIR 		?= objects
OUTPUT_DIR 		?= OUTPUT
TARGET_NAME		?= HexaFPV_app
ARCH			?= ARM
# ARCH			?= x64
# ARCH			?= x86

TARGET 			= $(OUTPUT_DIR)/$(TARGET_NAME)


UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  $(info Linux build)
	GCC_PATH 	=
	EXEC_SUFIX 	= .elf
	NUM_CORES	?= $(shell nproc)
else ifeq ($(UNAME_S),MINGW64_NT-10.0-19045)
  $(info Windows Bash build)
	GCC_PATH 	=
	EXEC_SUFIX 	= .exe
	NUM_CORES	?= $(shell wmic cpu get NumberOfLogicalProcessors | grep -vE "^[^0-9]+$ ")
else
	$(info UNAME_S is $(UNAME_S))
	$(info Unknown build)
endif

$(info NUM_CORES count $(NUM_CORES))
MAKEFLAGS += -j$(NUM_CORES)

CC 	= $(GCC_PATH)gcc
CXX = $(GCC_PATH)g++

GIT_BRANCH 		:= $(shell git rev-parse --abbrev-ref HEAD)
GIT_COMMIT_HASH := $(shell git rev-parse HEAD)
GIT_INFO 		= -DGIT_BRANCH=\"$(GIT_BRANCH)\" -DGIT_COMMIT_HASH=\"$(GIT_COMMIT_HASH)\"

COMONFLAGS 		= $(GIT_INFO) -g

# Language Specific Flags
CFLAGS		= $(COMONFLAGS) -Wall -g -std=gnu11
CXXFLAGS 	= $(COMONFLAGS) -std=c++11
ASMFLAGS 	= $(COMONFLAGS) -c -x assembler-with-cpp

# Linker Flags
LDFLAGS		= -Wl,-Map=$(TARGET).map \
			-lstdc++ \
			-lm
#LDFLAGS 	= -L../lib/lib/$(ARCH)/ \
			-lSDL2 \
			-lstdc++ \
			-Wl,-Map="$(TARGET).map"
# -lSDL2test
# -I../lib/include
# -lSDL2main -lSDL2test


SRCS=
INCLUDE_PATHS=
# DEFINE_LIST=
include list_filelist.mk
SRCS_C 		= $(filter %.c, $(SRCS))
OBJS_C 		= $(SRCS_C:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

SRCS_CPP 	= $(filter %.cpp, $(SRCS))
OBJS_CPP 	= $(SRCS_CPP:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

SRCS_S 		= $(filter %.s, $(SRCS))
OBJS_S 		= $(SRCS_S:$(SRC_DIR)/%.s=$(OBJ_DIR)/%.o)

OBJS 		+= $(OBJS_CPP) $(OBJS_C) $(OBJS_S)
# $(info OBJS:$(OBJS))

INCLUDES=$(addprefix -I,$(INCLUDE_PATHS))
# include list_defines.mk
    DEFINES =
    DEFINES += $(foreach d, $(DEFINE_LIST), -D$d)


all: $(TARGET_NAME)$(EXEC_SUFIX)

$(TARGET_NAME)$(EXEC_SUFIX): $(OBJS)
	@mkdir -p $(OUTPUT_DIR)
	@$(CC) -o $(TARGET)$(EXEC_SUFIX) $(OBJS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo '### Building C target: $@'
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo '### Building C++ target: $@'
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) $(DEFINES) $(INCLUDES) $(INCLUDES_CXX) -c $< -o "$@"

# @$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@


clean:
	@echo "Clean $(TARGET_NAME)"
	@rm -rf $(OBJ_DIR) OUTPUT

fresh: clean
	make -j$(NUM_CORES) $(TARGET_NAME)$(EXEC_SUFIX)

include extar_goals.mk
