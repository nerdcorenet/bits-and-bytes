/*
 * file2int.c - This program opens the file given on the command line,
 * and reads it as a single binary digit. file2int can output this
 * binary integer in any base 2-62. See the GMP documentation for
 * mpz_out_str() for details about numeric base.
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
#include <stdlib.h>
#include <libgen.h> // For basename()
#include "gmp.h" // Requires libgmp http://www.gmplib.org/

void usage(char*);

int main (int argc, char *argv[]) {
  // Setup variables
  unsigned char *filename, *ferrmsg, *filemode = "rb";
  FILE *file;
  unsigned int base;
  char *cmd = basename(argv[0]);
  // x will hold the total
  // xx is a temporary mpz_t integer used during the fread() loop
  // tfs is just the value 256 as a GMP mpz_t integer
  mpz_t x, xx, tfs;
  mpz_init(x);
  mpz_init(xx);
  mpz_init(tfs);
  mpz_set_ui(tfs, (unsigned long int)256);
  // onebyte is used to read one byte at a time from the input file
  unsigned char onebyte;

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

  // Set the user-defined base, or default to 10 (decimal)
  if (argc > 2) {
    //base = atoi(argv[1]);
    int bufferquesize = 0;
    int i;
    for (i = 0; argv[1][i] != '\0'; ++i) {
      bufferquesize *= 10;
      bufferquesize += argv[1][i] - '0';
    }
    base = bufferquesize;
  } else {
    base = 10;
  }

  // To get started, we read a single byte.
  if (fread(&onebyte, 1, 1, file) != 1) {
    if (feof(file)) {
      sprintf(ferrmsg, "File is empty.");
    } else if (ferror(file)) {
      sprintf(ferrmsg, "A file read error occured.");
    } else {
      sprintf(ferrmsg, "An unknown error occured.");
    }
    fprintf(stderr, "ERROR Unable to read from file %s: %s", filename, ferrmsg);
    exit(-1);
  }

  // Start at 0
  mpz_set_ui(x, 0);
  // Put the first byte into x
  mpz_add_ui(xx, x, (unsigned long int)onebyte);
  mpz_set(x, xx);

  // This loop reads one byte from file and adds it to x
  // Since x is a complex mpz_t integer and not a simple int
  // we cannot simply bitshift "<< 8" before adding onebyte to x,
  // so we multiply x by 256 to accomplish a bit shift
  // equivalent to "x << 8"
  while (fread(&onebyte, 1, 1, file) == 1) {
    mpz_mul(xx, x, tfs);
    mpz_set(x, xx);
    // After "bit shifting" x left by 8, we can simply add the value
    // of onebyte into x
    mpz_add_ui(x, x, (unsigned long int)onebyte);
  }

  fprintf(stderr, "Outputting file in base %d\n", base);

  mpz_out_str(stdout, base, x);
  printf("\n");

  exit(0);
}

// Prints usage summary and exits
void usage(char* cmd) {
  printf("Usage: %s FILENAME [BASE]\n\n", cmd);
  printf("  Read FILENAME and display it on STDOUT as a single unsigned integer of base BASE.\n");
  printf("  BASE is optional. Default is base 10.\n");

  exit(-1);
}
