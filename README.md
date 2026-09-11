# bits-and-bytes

A collection of small programs to deal with bits and bytes in a variety of ways.

## b2flop

b2flop calculates the height of a stack of DOS-formatted floppy disks required to store N bytes.

## bitcat

bitcat is a small C application to print the stream of bits comprising an input such as a file. It can print in 8-bit (1-byte) columns or unseparated stream of bits, and can output either "01" or ".-" (Morse Code) output.

## bitcount

bitcount is a small C application to count the bits (zeroes and ones) of an input stream such as a file. It displays statistical data such as the longest consecutive string of 0s and 1s, as well as the overall 0:1 ratio for the input.

## bwrite

bwrite is a small C application to write a single byte, by its decimal integer value (0-255) into a file. It asks the user for the decimal integer value of the byte to write, and can either write a new file or append the single byte to an existing file with the '-a' option.

## bytecount

Like bitcount, bytecount collects and displays some byte-level heuristics about an input file, such as number of occurances of each byte (0x00 - 0xFF) and number of unique bytes.

## int2file / file2int

int2file uses the [GMP library](http://www.gmplib.org/) to read a decimal integer from stdin and write the integer as binary to an output file. This is meant to act as a proof-of-concept that in fact all computer files may in fact be represented as a single number. It may also be used to convert numbers to files, such as the [Illegal Prime](https://en.wikipedia.org/wiki/Illegal_prime)

file2int reads the file specified on the command line and prints it to STDOUT. It accepts an optional command line argument to represent the numeric base which can range from 2-62 (default 10).

## intinverter

intinverter prompts the user for a number (int), prints the hex and the binary representations of that int, then negates the number (i.e. "0-n") and prints those. The hex output will be architecture-dependent becuase it uses printf(), but the binary output should always be in network byte order. No bounds checking is performed so numbers may be limited to (2^32)-1 depending on architecture/compiler.
