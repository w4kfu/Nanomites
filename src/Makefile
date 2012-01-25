include ../makefile.rules

OBJS = $(FILES:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LFLAGS) $^ -o ../$@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

nano: clean
	clang++ -D __STDC_LIMIT_MACROS=1 -D __STDC_CONSTANT_MACROS=1 `llvm-config --cxxflags` -fexceptions -o main.o -c main.cpp
	clang++ -D __STDC_LIMIT_MACROS=1 -D __STDC_CONSTANT_MACROS=1 `llvm-config --cxxflags` -fexceptions -o dispatch.o -c dispatch.cpp
	clang++ `llvm-config --ldflags` -o ../nano dispatch.o main.o `llvm-config --libs`
clean:
	rm -f *~ *.o
	rm -rf ../nano