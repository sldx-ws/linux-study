.PHONY:all
all:myproc test 

myproc:myproc.c 
	g++ -o $@ $^
test:test.c
	gcc -o $@ $^ 

.PHONY:clean 
clean: 
	rm -f test myproc
