all: dist/tap

dist/tap: event-queue.c interop.c main.c
	mkdir -p dist
	gcc -g event-queue.c interop.c attributes.c main.c -DDEBUG=$(DEBUG) -o $@

clean:
	rm -v dist/* || true
