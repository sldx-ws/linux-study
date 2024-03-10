test:test.o
	gcc -o $@ $^
test.o:test.s
	gcc -c -o $@ $^
test.s:test.i
	gcc -S -o $@ $^
test.i:test.c
	gcc -E -o $@ $^

.PHONY:clean
clean:
	rm -f test
