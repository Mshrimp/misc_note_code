

all:
	gcc -c debug.c
	gcc -c func.c
	#nm main | grep test_ > main.sym
	objdump -t func.o | grep test_ > debug.sym
	gcc symbol_tool.c -o symbol_tool
	./symbol_tool
	gcc -c symbol.c
	gcc debug.o func.o symbol.o -o debug

clean:
	rm *.o -rf
	-rm debug.sym -rf
	-rm symbol.c -rf
	-rm symbol debug symbol_tool -rf
