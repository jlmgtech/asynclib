CC = gcc
INCLUDES = -I./src/include
CFLAGS = -g $(INCLUDES)
MEMTEST = valgrind --leak-check=full --error-exitcode=1 --log-fd=9 9>>memlog

.PHONY: memlog clean test dist/tests/*

all: preamble dist/lib/libasync.a

install: all
	cp -r src/include/async/ /usr/local/include/async/
	cp dist/lib/libasync.a /usr/local/lib/libasync.a
	ldconfig

preamble:
	mkdir -p dist/tests
	mkdir -p dist/lib

test: memlog dist/tests/emitter dist/tests/events dist/tests/generators dist/tests/structures dist/tests/await
	# # # # # # # # # # # # # # # # # # # #
	# ALL TESTS AND MEMORY CHECKS PASSED! #
	# # # # # # # # # # # # # # # # # # # #

memlog:
	echo > memlog

dist/tests/await: src/tests/await.c dist/lib/libasync.a
	$(CC) -o $@ $^ -pthread -lffcall $(CFLAGS)
	./$@
	$(MEMTEST) ./$@

dist/tests/emitter: src/tests/eventemitter.c dist/lib/libasync.a
	$(CC) -o $@ $^ $(CFLAGS)
	./$@
	$(MEMTEST) ./$@

dist/tests/events: src/tests/events.c dist/lib/libasync.a
	$(CC) -o $@ $^ -pthread -lrt $(CFLAGS)
	./$@
	$(MEMTEST) ./$@

dist/tests/generators: src/tests/generators.c dist/lib/libasync.a
	$(CC) -o $@ $^ $(CFLAGS)
	./$@
	$(MEMTEST) ./$@

dist/tests/structures: src/tests/structures.c dist/lib/libasync.a
	$(CC) -o $@ $^ $(CFLAGS)
	./$@
	$(MEMTEST) ./$@

dist/lib/Array.o: src/lib/Array.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/Emitter.o: src/lib/Emitter.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/Events.o: src/lib/Events.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/Generator.o: src/lib/Generator.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/HashMap.o: src/lib/HashMap.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/Promise.o: src/lib/Promise.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/StringSet.o: src/lib/StringSet.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/Task.o: src/lib/Task.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/partial.o: src/lib/Partial.c
	$(CC) -c -o $@ $^ $(CFLAGS)

dist/lib/libasync.a: dist/lib/Array.o dist/lib/Emitter.o dist/lib/Events.o dist/lib/Generator.o dist/lib/HashMap.o dist/lib/Promise.o dist/lib/StringSet.o dist/lib/Task.o dist/lib/partial.o
	ar -rc $@ $^

clean:
	rm dist/tests/*
	rm dist/lib/*
