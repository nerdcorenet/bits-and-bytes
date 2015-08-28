/*
 * int2file.c - A program to convert arbitrarily long decimal integers
 * to raw binary data and write it to a file.
 *
 * This program requires the GMP library: http://libgmp.org/
 * 
 * Copyright (c) 2015 Mike Mallett <mike@nerdcore.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h> // For exit()
#include <libgen.h> // For basename()
#include "gmp.h" // Requires libgmp http://www.gmplib.org/

void usage(char*);

int main (int argc, char *argv[]) {
  // Setup variables
  unsigned char *filename, *filemode = "wb";
  FILE *file;
  FILE *tempfile = tmpfile();
  unsigned int base;
  unsigned long length;
  char *cmd = basename(argv[0]);
  mpz_t x;
  mpz_init(x);

  // Process command-line args
  if (argc < 2) {
    usage(cmd);
  }

  // Open file
  filename = *(++argv);
  file = fopen(filename, filemode);
  if (file == NULL) {
    printf("Unable to open file %s\n", filename);
    exit(-1);
  }

  // Read STDIN into x
  mpz_inp_str(x, stdin, 0);

  // Write x as a single binary number
  mpz_out_raw(tempfile, x);

  // The call to mpz_out_raw() includes a prefix of 4 bytes
  // in order to make it readable by mpz_in_raw(). Here we
  // write it to tempfile, determine the size of tempfil minus 4,
  // and use this value to get the unsigned raw binary data
  // without a prefix/header.
  length = ftell(tempfile) - 4;

  // We need a place to store data from tempfile
  unsigned char holder[length+1];

  // Skip 4 bytes at the beginning of tempfile
  fseek(tempfile, 4, SEEK_SET);

  // Read all remaining data into holder
  fread(&holder, 1, length, tempfile);

  // Write the output file with the data from holder
  fwrite(&holder, 1, length, file);

  fclose(file);

  exit(0);
}

// Prints usage summary and exits
void usage(char* cmd) {
  printf("Usage: %s FILENAME\n\n", cmd);
  printf("  Read decimal integer input from the user via STDIN, and output the value as unsigned binary to FILENAME.\n");

  exit(-1);
}
