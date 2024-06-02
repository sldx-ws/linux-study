libmymath.a:my_add.o my_sub.o
	ar -rc $@ $^
my_add.o:my_add.c
	gcc -c $^ -o $@
my_sub.o:my_sub.c
	gcc -c $^ -o $@

.PHONY:output
output:
	mkdir -p mylib/include
	mkdir -p mylib/lib 
	cp -f *.a mylib/lib
	cp -r *.h mylib/include
.PHONY:clean
clean:
	rm -f *.o libmymath.a
