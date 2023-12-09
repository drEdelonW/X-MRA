CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-Wl,-Map=$(TARGET).map -lX11 -lstdc++ -lm 
SRC_DIR=src

SRCS=
include filelist.mk
OBJS=$(SRCS:$(SRC_DIR)/%.c=objects/%.o)

INCLUDE_PATHS=
include include_paths.mk
INCLUDES=$(addprefix -I,$(INCLUDE_PATHS))

TARGET=OUTPUT/HexaFPV_main_app

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p OUTPUT
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LDFLAGS)

objects/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objects/%.o: $(SRC_DIR)/%.cpp
	mkdir -p objects
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf objects OUTPUT
