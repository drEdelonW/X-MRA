CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-Wl,-Map=$(TARGET).map,-lX11
SRC_DIR=src

SRCS=
include filelist.mk
OBJS=$(SRCS:$(SRC_DIR)/%.c=objects/%.o)

INCLUDE_PATHS=
include include_paths.mk
INCLUDES=$(addprefix -I,$(INCLUDE_PATHS))
# INCLUDES += -lX11

TARGET=OUTPUT/hexaFPV

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p OUTPUT
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LDFLAGS)

objects/%.o: $(SRC_DIR)/%.c
	mkdir -p objects
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf objects OUTPUT
