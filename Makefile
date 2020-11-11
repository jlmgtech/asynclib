dist/libjslib.a: preamble test
	ar -rc $@ dist/lib/*.o

preamble:
	mkdir -p dist/tests
	mkdir -p dist/lib

test: dist/tests/emitter dist/tests/events dist/tests/generators dist/tests/structures
	# TESTS DONE #

dist/tests/await: src/tests/await.c dist/lib/*.o
	gcc -o $@ $^ -pthread -lffcall
	./$@

dist/tests/emitter: src/tests/eventemitter.c dist/lib/Emitter.o dist/lib/Array.o dist/lib/HashMap.o dist/lib/StringSet.o
	gcc -o $@ $^
	./$@

dist/tests/events: src/tests/events.c dist/lib/Events.o dist/lib/Task.o dist/lib/Promise.o
	gcc -o $@ $^ -pthread -lrt
	./$@

dist/tests/generators: src/tests/generators.c dist/lib/Generator.o
	gcc -o $@ $^
	./$@

dist/tests/structures: src/tests/structures.c dist/lib/HashMap.o dist/lib/StringSet.o dist/lib/Array.o
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

clean:
	rm dist/tests/*
	rm dist/lib/*
	rm dist/libjslib.a
