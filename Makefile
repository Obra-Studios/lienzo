# Lienzo Makefile
# Requires Emscripten SDK to be installed and activated

EMCC = emcc
EMCC_FLAGS = -std=c++17 -O2 -s WASM=1 -s USE_WEBGL2=1 -s ALLOW_MEMORY_GROWTH=1 \
	-s EXPORTED_FUNCTIONS='["_create_canvas","_create_frame","_add_frame_to_canvas","_main",\
	"_crdt_manager_create","_crdt_manager_get",\
	"_crdt_create_frame","_crdt_frame_get_x","_crdt_frame_get_y","_crdt_frame_get_width","_crdt_frame_get_height",\
	"_crdt_frame_set_position","_crdt_frame_set_size","_crdt_frame_delete","_crdt_get_all_frames",\
	"_crdt_create_rectangle","_crdt_rectangle_get_x","_crdt_rectangle_get_y","_crdt_rectangle_get_width","_crdt_rectangle_get_height",\
	"_crdt_rectangle_set_position","_crdt_rectangle_set_size","_crdt_rectangle_delete","_crdt_get_all_rectangles",\
	"_crdt_create_textbox","_crdt_textbox_get_text","_crdt_textbox_set_text","_crdt_get_all_textboxes",\
	"_crdt_free_string","_malloc","_free"]' \
	-s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString"]'

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

