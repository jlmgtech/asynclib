all: preamble dist/lib/libasync.a test

preamble:
	mkdir -p dist/tests
	mkdir -p dist/lib

test: dist/tests/emitter dist/tests/events dist/tests/generators dist/tests/structures
	# TESTS DONE #

dist/tests/await: src/tests/await.c dist/lib/libasync.a
	gcc -o $@ $^ -pthread -lffcall
	./$@

dist/tests/emitter: src/tests/eventemitter.c dist/lib/libasync.a
	gcc -o $@ $^
	./$@

dist/tests/events: src/tests/events.c dist/lib/libasync.a
	gcc -o $@ $^ -pthread -lrt
	./$@

dist/tests/generators: src/tests/generators.c dist/lib/libasync.a
	gcc -o $@ $^
	./$@

dist/tests/structures: src/tests/structures.c dist/lib/libasync.a
	gcc -o $@ $^
	./$@

dist/lib/Array.o: src/lib/Array.c
	gcc -c -o $@ $^

dist/lib/Emitter.o: src/lib/Emitter.c
	gcc -c -o $@ $^

dist/lib/Events.o: src/lib/Events.c
	gcc -c -o $@ $^

dist/lib/Generator.o: src/lib/Generator.c
	gcc -c -o $@ $^

dist/lib/HashMap.o: src/lib/HashMap.c
	gcc -c -o $@ $^

dist/lib/Promise.o: src/lib/Promise.c
	gcc -c -o $@ $^

dist/lib/StringSet.o: src/lib/StringSet.c
	gcc -c -o $@ $^

dist/lib/Task.o: src/lib/Task.c
	gcc -c -o $@ $^

dist/lib/partial.o: src/lib/partial.c
	gcc -c -o $@ $^

dist/lib/libasync.a: dist/lib/Array.o dist/lib/Emitter.o dist/lib/Events.o dist/lib/Generator.o dist/lib/HashMap.o dist/lib/Promise.o dist/lib/StringSet.o dist/lib/Task.o dist/lib/partial.o
	ar -rc $@ $^

clean:
	rm dist/tests/*
	rm dist/lib/*
	rm dist/libjslib.a
