# Whatever is going on in here, it works.
TARGET_EXEC = LabSchrodinger
CC = gcc
LDFLAGS = -lm -llapacke

BUILD_DIR = ./build
SRC_DIRS = ./src
SRCS = $(shell find $(SRC_DIR) -name *.c)
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)
INC_DIR = $(shell find $(SRC_DIRS) -type d)
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

CFLAGS = -O3 -pedantic -Wall

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(DEPS)
