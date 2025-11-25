# Lienzo Makefile
# Requires Emscripten SDK to be installed and activated

EMCC = emcc
EMCC_FLAGS = -std=c++17 -O2 -s WASM=1 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS='["_create_canvas","_create_frame","_add_frame_to_canvas","_main"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]'

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/**/**/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean build

all: build

build: $(BUILD_DIR)/lienzo.js

$(BUILD_DIR)/lienzo.js: $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(EMCC) $(EMCC_FLAGS) $(OBJECTS) -o $(BUILD_DIR)/lienzo.js

%.o: %.cpp
	@mkdir -p $(dir $@)
	$(EMCC) $(EMCC_FLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	find $(SRC_DIR) -name "*.o" -delete

