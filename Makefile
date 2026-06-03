# =============================================================================
#  Makefile - Cod Rutier (Simulare de trafic rutier)
#  Proiect POO C++ - compilabil cu g++ pe Linux (Ubuntu).
# -----------------------------------------------------------------------------
#  Tinte disponibile:
#    make          -> compileaza jocul (binarul ./cod_rutier)
#    make run      -> compileaza si ruleaza jocul
#    make test     -> compileaza si ruleaza testele unitare
#    make clean    -> sterge fisierele obiect si binarele
# =============================================================================

CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -Iinclude
TARGET    := cod_rutier

# Toate sursele din src/ formeaza jocul (cu exceptia eventualelor main-uri de test).
SRC       := $(wildcard src/*.cpp)
OBJ       := $(SRC:.cpp=.o)

# Sursele jocului FARA main.cpp (refolosite la legarea testelor).
SRC_NOMAIN := $(filter-out src/main.cpp,$(SRC))
OBJ_NOMAIN := $(SRC_NOMAIN:.cpp=.o)

# Testele: fiecare fisier din tests/ devine un binar separat in build/tests.
TEST_SRC  := $(wildcard tests/*.cpp)
TEST_BIN  := $(patsubst tests/%.cpp,build/tests/%,$(TEST_SRC))

# --- Compilare joc -----------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

# --- Teste unitare -----------------------------------------------------------
test: $(TEST_BIN)
	@echo "=== Rulez testele unitare ==="
	@for t in $(TEST_BIN); do echo "--> $$t"; $$t || exit 1; done
	@echo "=== Toate testele au trecut ==="

build/tests/%: tests/%.cpp $(OBJ_NOMAIN)
	@mkdir -p build/tests
	$(CXX) $(CXXFLAGS) -o $@ $< $(OBJ_NOMAIN)

# --- Curatenie ---------------------------------------------------------------
clean:
	rm -f $(OBJ) $(OBJ_NOMAIN) $(TARGET)
	rm -rf build

.PHONY: all run test clean
