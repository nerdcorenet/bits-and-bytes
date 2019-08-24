all: b2flop bitcat bitcount bytecount bwrite file2int int2file

b2flop: b2flop.c
	gcc -o b2flop b2flop.c -lgmp

bitcat: bitcat.c
	gcc -o bitcat bitcat.c

bitcount: bitcount.c
	gcc -o bitcount bitcount.c

bytecount: bytecount.c
	gcc -o bytecount bytecount.c

bwrite: bwrite.c
	gcc -o bwrite bwrite.c

file2int: file2int.c
	gcc -o file2int file2int.c -lgmp

int2file: int2file.c
	gcc -o int2file int2file.c -lgmp

clean:
	$(RM) b2flop bitcat bitcount bytecount bwrite file2int int2file
