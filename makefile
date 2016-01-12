
## Rules ###############################################################

.PHONY: all
all: jansen



## Clean ###############################################################
.PHONY: clean
clean:
	$(RM) *.o *~



#exec
jansen: jansen.o structure.o input.o queue.o
	gcc jansen.o structure.o input.o queue.o -o jansen


# ROOT
jansen.o: jansen.c
	gcc -Wall -ansi -pedantic -Wno-unused-result -c jansen.c -g




# COMMON
input.o: input.c input.h
	gcc -Wall -ansi -pedantic -Wno-unused-result -c input.c -g
structure.o: structure.c structure.h
	gcc -Wall -ansi -pedantic -Wno-unused-result -c structure.c -g
queue.o: list/queue.c list/queue.o
	gcc -Wall -ansi -pedantic -Wno-unused-result -c list/queue.c -g
