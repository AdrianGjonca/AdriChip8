CC := gcc

SOURCE_DIR := SOURCE
OBJECT_DIR := OBJECT

SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

EXECUTABLE := adrichip8

# SDL2 configuration
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS := $(shell sdl2-config --libs)

.PHONY: all clean

all: $(EXECUTABLE)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(OBJECT_DIR)
	$(CC) $(SDL2_CFLAGS) -c $< -o $@

$(EXECUTABLE): $(OBJECT_FILES)
	$(CC) $^ $(SDL2_LIBS) -o $@

$(OBJECT_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJECT_DIR) $(EXECUTABLE)
