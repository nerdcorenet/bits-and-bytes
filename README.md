# bits-and-bytes

A collection of small programs to deal with bits and bytes in a variety of ways.

## bitcount

bitcount is a small C application to count the bits (zeroes and ones) of an input stream such as a file. It displays statistical data such as the longest consecutive string of 0s and 1s, as well as the overall 0:1 ratio for the input.

## bitcat

bitcat is a small C application to print the stream of bits comprising an input such as a file. It can print in 8-bit (1-byte) columns or unseparated stream of bits, and can output either "01" or ".-" (Morse Code) output.

## bwrite

bwrite is a small C application to write a single byte, by its decimal integer value (0-255) into a file. It asks the user for the decimal integer value of the byte to write, and can either write a new file or append the single byte to an existing file with the '-a' option.

