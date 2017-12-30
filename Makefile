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

# Source files
SRC_DIR = src
SRC = $(shell find $(SRC_DIR) -type f -name '*.cpp')

# Dependency files
DEP_DIR = deps
DEP_FILES = $(SRC:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)

# Object files
BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Dummy warning files
WARNINGS = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.w)

# Unit test paths
SRC_DIR_TEST = tests
SRC_TEST = $(shell find $(SRC_DIR_TEST) -type f -name 'test_*.hpp') # Test files
TEST_CPP = $(SRC_DIR_TEST)/tests.cpp
TEST_EXEC = $(SRC_DIR_TEST)/tests

# Executable name
EXEC = dungeon-battle

# Documentation directory
DOC_DIR = doc

# Dir where to output cppcheck reports
CHECK_DIR = check

.PHONY: all release debug tests-compile tests clean clean-all warnings lint cppcheck-html doc package package-deb package-tar
NO_DEPS = clean clean-all warnings lint cppcheck-html doc package package-deb package-tar $(TEST_EXEC)

debug: DFLAGS += -ggdb
debug: $(EXEC)

release: CFLAGS += -O3 -DNDEBUG
release: $(EXEC)

all: release doc cppcheck-html test

warnings: CFLAGS += -fsyntax-only
warnings: WFLAGS += $(WFLAGS_EXTRA)
warnings: $(WARNINGS)

# Include dependency files, without errors if they do not exist
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NO_DEPS))))
        -include $(DEP_FILES)
endif

# Build executable from object files
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(DFLAGS) $(LFLAGS)

# Build object file from source file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	@mkdir -p $(dir $@)
	$(CXX) -o $@ -c $< $(CFLAGS) $(DFLAGS) $(WFLAGS)

# Output warnings
$(BUILD_DIR)/%.w: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< $(CFLAGS) $(WFLAGS)

# Create dependency files
$(DEP_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -MM -MT '$(<:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)' $< -MF $@


# Generate tests with cxxtest
$(TEST_CPP): $(SRC_TEST)
	cxxtestgen --error-printer -o tests/test.cpp $^

# Compile test without executing them
tests-compile: $(TEST_CPP) $(filter-out $(BUILD_DIR)/main.o,$(OBJ))
	$(CXX) -o $(TEST_EXEC) $^ $(CFLAGS) $(WFLAGS) $(LFLAGS)

# Compile and execute tests using cxxtest
tests: tests-compile
	$(TEST_EXEC) -v

# Static analysis of the code, cause make to fail if an error is found
lint:
	cppcheck -q --enable=all --suppressions-list=.cppignore --inconclusive $(SRC_DIR) 1> /dev/null
	@# Return exit code 1 if there are errors
	cppcheck --error-exitcode=2 --suppressions-list=.cppignore --inconclusive $(SRC_DIR) 1> /dev/null 2> /dev/null

# Generate linter's report
cppcheck-html:
	cppcheck --enable=all --suppressions-list=.cppignore --inconclusive --xml --xml-version=2 $(SRC_DIR) 2> tmp_cppcheck.xml
	cppcheck-htmlreport --file=tmp_cppcheck.xml --report-dir=$(CHECK_DIR) --source-dir=.
	@rm tmp_cppcheck.xml

# Generate the documentation
doc:
	doxygen .doxygen.conf


# Create all packages
package:
	$(MAKE) package-deb
	$(MAKE) package-tar

# Create debian package
#.ONESHELL:
package-deb: CFLAGS += -s -DPACKAGE
package-deb:
	$(MAKE) clean-all
	$(MAKE) release

	cd packages
	mkdir -p for-debian/usr/share/doc/dungeon-battle
	cp -r DEBIAN for-debian
	gzip --best -nc changelog > for-debian/usr/share/doc/dungeon-battle/changelog.gz
	cp copyright for-debian/usr/share/doc/dungeon-battle

	mkdir -p for-debian/usr/games
	strip ../dungeon-battle
	cp ../dungeon-battle for-debian/usr/games

	mkdir -p for-debian/usr/share/dungeon-battle
	cp -r ../data/* for-debian/usr/share/dungeon-battle

	fakeroot dpkg-deb --build for-debian dungeon-battle.deb

# Create portable version
#.ONESHELL:
package-tar:
	$(MAKE) clean-all
	$(MAKE) release

	cd packages
	mkdir -p for-tar
	cp changelog for-tar
	cp copyright for-tar

	strip ../dungeon-battle
	cp ../dungeon-battle for-tar
	cp -r ../data for-tar

	tar -czf dungeon-battle.tar.gz for-tar --transform s/for-tar/dungeon-battle/


# Clean the workspace, except executables and documentation
clean:
	rm -rf $(BUILD_DIR) $(BUILD_DIR)/generation
	rm -rf $(CHECK_DIR)
	rm -rf $(DEP_DIR)
	rm -rf packages/for-debian packages/for-tar
	rm -rf tests/test.cpp
	rm -rf *~

# Clean everything
clean-all: clean
	rm -rf dungeon-battle
	rm -rf packages/dungeon-battle.deb packages/dungeon-battle.tar.gz
	rm -rf $(DOC_DIR)
	rm -rf $(TEST_EXEC)
