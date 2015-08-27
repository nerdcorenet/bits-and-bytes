# bits-and-bytes

A collection of small programs to deal with bits and bytes in a variety of ways.

## bitcount

bitcount is a small C application to count the bits (zeroes and ones) of an input stream such as a file. It displays statistical data such as the longest consecutive string of 0s and 1s, as well as the overall 0:1 ratio for the input.

## bitcat

bitcat is a small C application to print the stream of bits comprising an input such as a file. It can print in 8-bit (1-byte) columns or unseparated stream of bits, and can output either "01" or ".-" (Morse Code) output.

## bwrite

bwrite is a small C application to write a single byte, by its decimal integer value (0-255) into a file. It asks the user for the decimal integer value of the byte to write, and can either write a new file or append the single byte to an existing file with the '-a' option.

## int2file

in2file uses the [GMP library](http://www.gmplib.org/) to read a decimal integer from stdin and write the integer as binary to an output file. This is meant to act as a proof-of-concept that in fact all computer files may in fact be represented as a single number. It may also be used to convert numbers to files, such as the [Illegal Prime](https://en.wikipedia.org/wiki/Illegal_prime)

If int2file is executed as `file2int` it does the opposite - It reads the file specified on the command line and prints it to STDOUT using the optional second command line argument to represent the numeric base which can range from 2-62 (default 10, AKA "decimal").

int2file.c uses libgmp and must be compiled with `-lgmp` such as:

`
gcc -o int2file int2file.c -lgmp
ln int2file file2int
`
