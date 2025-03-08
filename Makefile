SOURCES_CPP  := $(shell find * -name '*.cpp')

HEADERS_CPP  := $(shell find * -name '*.hpp')	

BUILD_DIR    := build
OUTPUT 		 := board

OBJECTS_CPP  := $(addprefix build/,$(SOURCES_CPP:.cpp=.o))
SRC_DIRS     := $(shell find src -type d)

SFML_PATH 	 := ../lib/SFML/SFML

COMPILER_FLAGS_CPP 	 := -Wall -Wextra -I$(SFML_PATH)/include -std=c++23

LINKER_FLAGS := -L$(SFML_PATH)/lib -lsfml-system -lsfml-graphics -lsfml-window

COMPILER_CPP := $(shell command -v clang++ || command -v g++)

ifeq ($(strip $(COMPILER_CPP)),)
  $(error Neither GNU's C++ compiler, nor LLVM's clang++ compiler ar available. Build failed!)
endif


all: debug

# Create build directory if it does not exist
mkbuilddir:
	$(shell mkdir -p $(BUILD_DIR))
	$(foreach dir,$(SRC_DIRS),$(shell mkdir -p $(BUILD_DIR)/$(dir)))

# Debug build
debug: COMPILER_FLAGS_CPP += $(COMPILER_FLAGS_DEBUG)
debug: mkbuilddir $(OUTPUT)

release: COMPILER_FLAGS_CPP += $(COMPILER_FLAGS_RELEASE)
release: mkbuilddir $(OUTPUT)


$(BUILD_DIR)/%.o: %.cpp $(HEADERS_CPP)
	$(COMPILER_CPP) $(COMPILER_FLAGS_CPP) -c $< -o $@

# Link object files into the final executable
$(OUTPUT): $(OBJECTS_CPP)
	$(COMPILER_CPP) $(COMPILER_FLAGS_CPP) $(OBJECTS_CPP) $(LINKER_FLAGS) -o $(OUTPUT)

# Clean up any previous build
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(OUTPUT)

.PHONY: all debug release clean
