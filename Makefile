include makefile.rules

all:

nano:
	cd ./src/ && make nano

test: nano
	cd ./test && clang test.c -o test.ll  -S -emit-llvm
	cd ./test && llvm-as test.ll
	cd ./test && llc test.bc
	@echo
	@echo "[+] Launching test"
	@echo
	./nano ./test/test.ll ./test/output.ll
	@echo
	@echo "[+] Make Diff"
	@echo
	diff ./test/test.ll ./test/output.ll
clean:
	cd ./src/ && make clean

distclean: clean
	rm -f makefile.rules
	rm -f $(EXEC)