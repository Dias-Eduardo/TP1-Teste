# Author: Rafael Garibotti

##### Compiler options #####
CC = gcc
CFLAGS = -c -Wall

##### Project specific libraries #####
SRC_FILES += $(wildcard src/*.c)
TEST_FILES += $(wildcard test/*.c)

##### General commands #####
all: $(SRC_FILES) $(TEST_FILES)
	$(CC) $(CFLAGS) $^
	$(CC) *.o -o app
	rm -rf *.o

#all: app

#app: main.o aes.o camellia.o seed.o
#	$(CC) main.o aes.o camellia.o seed.o -o app
#	rm -rf *.o

#main.o: test/main.c
#	$(CC) $(CFLAGS) $^

#aes.o: src/aes.c
#	$(CC) $(CFLAGS) $^

#camellia.o: src/camellia.c
#	$(CC) $(CFLAGS) $^

#seed.o: src/seed.c
#	$(CC) $(CFLAGS) $^

clean:
	rm -rf *.o app