# ==========================================
#   Unity Project - A Test Framework for C
#   Copyright (c) 2007 Mike Karlesky, Mark VanderVoord, Greg Williams
#   [Released under MIT License. Please refer to license.txt for details]
# ==========================================

#We try to detect the OS we are running on, and adjust commands as needed
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif
#CFLAGS que já estavam no makefile do Unity
CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition

#CFLAGS adicionadas 
CFLAGS += -Wfatal-errors

#Flags para o funcionamento do gcov
GCOV_FLAGS += -fprofile-arcs 
GCOV_FLAGS += -ftest-coverage

TARGET_BASE1=all_tests
TARGET1 = $(TARGET_BASE1)$(TARGET_EXTENSION)
SRC_FILES1=\
  test/unity.c \
  test/extras/fixture/src/unity_fixture.c \
  src/aes.c \
  src/camellia.c \
  src/seed.c \
  src/crypt.c \
  test/test.c \
  test/test_runners/TestFoo_Runner.c \
  test/test_runners/all_tests.c
  
CPPCHECK_SRC_FILES=\
  src/aes.c \
  src/camellia.c \
  src/seed.c \
  src/crypt.c 
  
 GCOV_FILES=\
  aes.gcno\
  camellia.gcno \
  seed.gcno \
  crypt.gcno 
  
INC_DIRS=-Isrc -Isrc -I/extras/fixture/src

#Compila e roda normalmente
all:	clean compile run 

gcov: clean
	$(C_COMPILER) $(CFLAGS) $(GCOV_FLAGS) $(INC_DIRS) $(SRC_FILES1) -fsanitize=address -o $(TARGET1) #Compila com as flags do GCOV
	- ./$(TARGET1) -v #run normal
	#--------RELATÓRIO DO GCOV------------------
	gcov -b $(GCOV_FILES)

cppcheck: clean
	cppcheck cppcheck --enable=all --suppress=missingIncludeSystem $(CPPCHECK_SRC_FILES) $(CPPCHECK_SRC_FILES) 

valgrind: clean 
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SRC_FILES1) -o $(TARGET1)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET1)

compile:
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SRC_FILES1) -fsanitize=address -o $(TARGET1)

run:
	- ./$(TARGET1) -v

clean:
	$(CLEANUP) $(TARGET1)
	rm -f *.o *.c.gcov *.gcda *.gcno
	
ci: CFLAGS += -Werror
ci: compile
