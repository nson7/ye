objects = program.o bill.o fred.o

edit : $(objects)
	gcc -o edit $(objects)

program.o : lib.h

.PHONY : clean
clean :
	-rm edit $(objects)
