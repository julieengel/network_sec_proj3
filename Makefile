all: cracker

cracker:
	clang -g cracker.c -o cracker -Wall

clean:
	rm -f cracker
.PHONY: clean