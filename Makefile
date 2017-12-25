# C++ compiler name
CXX = g++

# Compiler flags
CFLAGS = -std=c++14

# Debuguer flags
DFLAGS =

# Warning flags
WFLAGS = -Wall -Wextra
WFLAGS_EXTRA = -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization \
               -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs \
               -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow \
               -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef

# Linker flags
LFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
SRC = $(shell find src -type f -name '*.cpp') # List of files to compile

# Dependency files
DEP_DIR = deps
DEP_FILES = $(SRC:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)

BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# List of object files in build/

EXEC = dungeon-battle
# Executable name

# Documentation directory
DOC_DIR = doc

# Dir where to output cppcheck reports
CHECK_DIR = check

# Unit test paths
SRC_DIR_TEST = tests
SRC_TEST = $(SRC_DIR_TEST)/test_map.hpp \
           $(SRC_DIR_TEST)/test_kdtree.hpp \
           $(SRC_DIR_TEST)/test_config.hpp \
           $(SRC_DIR_TEST)/test_pattern.hpp

TEST_CPP = $(SRC_DIR_TEST)/test.cpp

TEST_EXEC = $(SRC_DIR_TEST)/test

.PHONY: all release debug test doc cppcheck-html clean
NO_DEPS = clean lint doc cppcheck-html warning $(TEST_EXEC)

debug: DFLAGS += -ggdb
debug: $(EXEC)

release: CFLAGS += -O3 -DNDEBUG
release: $(EXEC)

#warning: WFLAGS += $(WFLAGS_EXTRA)
#warning: CFLAGS += -fsyntax-only
#warning: $(OBJ)

all: release doc cppcheck-html test

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NO_DEPS))))
        # Include dependency files, without errors if they do not exist
        -include $(DEP_FILES)
endif

# Build executable from object files
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(DFLAGS) $(LFLAGS)

# Build object file from source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

# Create dependency files
$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -MM -MT '$(<:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)' $< -MF $@

# Build and execute test
$(SRC_DIR_TEST)/%: $(BUILD_DIR)/%.o
	$(CXX) -o $@ $< $(DFLAGS) $(LFLAGS)
	@echo "Running the test: " $(@:$(SRC_DIR_TEST)/%=%)
	@cd $(SRC_DIR_TEST) && ./$(@:$(SRC_DIR_TEST)/%=%)

lint:
	cppcheck -q --enable=all --suppressions-list=.cppignore --inconclusive $(SRC_DIR) 1> /dev/null
	@# Return exit code 1 if there are errors
	cppcheck --error-exitcode=2 --suppressions-list=.cppignore --inconclusive $(SRC_DIR) 1> /dev/null 2> /dev/null

# Exectutes tests using cxxtest
tests: $(TEST_CPP)
	$(CXX) -o $@ $< $(CFLAGS) $(WFLAGS)
	$(TEST_EXEC) -v

# Build the test cpp
$(TEST_CPP): $(SRC_TEST)
	cxxtestgen --error-printer -o tests/test.cpp $<

# Generate the documentation
doc:
	doxygen .doxygen.conf

# Generate linter's report
cppcheck-html:
	cppcheck --enable=all --suppressions-list=.cppignore --inconclusive --xml $(SRC_DIR) 2> tmp_cppcheck.xml
	cppcheck-htmlreport --file=tmp_cppcheck.xml --report-dir=$(CHECK_DIR) --source-dir=.
	@rm tmp_cppcheck.xml

# Create debian package
.ONESHELL:
package: CFLAGS += -s -DPACKAGE
package: release
	cd packages
	@# Build debian package
	@mkdir -p for-debian/usr/share/doc/dungeon-battle
	cp -r DEBIAN for-debian
	gzip --best -nc changelog > for-debian/usr/share/doc/dungeon-battle/changelog.gz
	cp copyright for-debian/usr/share/doc/dungeon-battle
	@# Add binaries
	@mkdir -p for-debian/usr/games
	strip ../dungeon-battle
	cp ../dungeon-battle for-debian/usr/games
	@# Add datas
	@mkdir -p for-debian/usr/share/dungeon-battle
	cp -r ../data/* for-debian/usr/share/dungeon-battle
	@# Build package
	dpkg-deb --build for-debian dungeon-battle.deb

# Clean the workspace, except executables and documentation
clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR)/generation
	rm -rf $(CHECK_DIR)
	rm -rf $(DEP_DIR)
	rm -rf packages/for-debian
	rm -rf tests/test.cpp
	rm -rf *~

# Clean everything
clean-all: clean
	rm -rf dungeon-battle
	rm -rf packages/dungeon-battle.deb
	rm -rf $(DOC_DIR)
	rm -rf $(EXEC_TEST)
