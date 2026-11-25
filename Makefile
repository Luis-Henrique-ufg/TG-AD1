CC     := gcc
CFLAGS := -Wall -Wextra -std=c11 -Iinclude

ifeq ($(OS),Windows_NT)
    EXE := .exe
    RM  := del /Q
else
    EXE :=
    RM  := rm -f
endif

TARGET   := ead$(EXE)
TEST_BIN := testar_tudo$(EXE)

CORE_SRC := src/cursos.c src/aulas.c src/ordenacao.c
ALL_SRC  := $(CORE_SRC) src/interface.c src/main.c

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(ALL_SRC)
	$(CC) $(CFLAGS) $(ALL_SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(CORE_SRC) tests/testar_tudo.c
	$(CC) $(CFLAGS) $(CORE_SRC) tests/testar_tudo.c -o $(TEST_BIN)
	./$(TEST_BIN)

clean:
	$(RM) $(TARGET) $(TEST_BIN) *.o
