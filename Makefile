include makefile.rules

all:
	cd ./src/ && make

clean:
	cd ./src/ && make clean

distclean: clean
	rm -f makefile.rules
	rm -f $(EXEC)