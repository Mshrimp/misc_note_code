

all:
	gcc main.c -o main
	#nm main | grep test_ > main.sym
	objdump -t main | grep test_ > main.sym
	gcc symbol_tool.c -o symbol_tool
	./symbol_tool
	gcc symbol.c -o symbol
