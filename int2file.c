/*
 * int2file.c - A program to convert arbitrarily long decimal integers
 * to raw binary data and write it to a file.
 *
 * If this file is invoked as "file2int", it does the opposite. It opens
 * the file given on the command line, and reads it as a single binary
 * digit. file2int can output this binary integer in any base 2-62. See
 * the GMP documentation for mpz_out_str() for details about numeric
 * base.
 *
 * This program requires the GMP library: http://libgmp.org/
 * and must be compiled with the "-lgmp" option.
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
#include <strings.h> // For strcmp()
#include "gmp.h" // Requires libgmp http://www.gmplib.org/

// These defines are used to determine, once, whether we are writing to
// or reading from the file specified on the command line.

#define _EXEC_FORM_UNKNOWN 0
#define _EXEC_FORM_INTFILE 1
#define _EXEC_FORM_FILEINT 2

void usage(int, char*);

void intfile(mpz_t, FILE*);
void fileint(FILE*, unsigned int);

int main (int argc, char *argv[]) {
  // Setup variables
  unsigned char *filename, *filemode;
  FILE *file;
  unsigned int base, execform;
  char *cmd = basename(argv[0]);
  mpz_t x;
  mpz_init(x);

  if (strcmp(cmd, "int2file") == 0) {
    execform = _EXEC_FORM_INTFILE;
    filemode = "wb";
  } else if (strcmp(cmd, "file2int") == 0) {
    execform = _EXEC_FORM_FILEINT;
    filemode = "rb";
  } else {
    execform = _EXEC_FORM_UNKNOWN;
    usage(execform, cmd);
  }

  // Process command-line args
  if (argc < 2) {
    usage(execform, cmd);
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
    base = (int)(argv[1][0] - '0');
  } else {
    base = 10;
  }

  // Now that the file is open for either reading or writing,
  // depending on invocation of this program, perform the
  // appropriate actions based on execform
  switch (execform) {
  case _EXEC_FORM_INTFILE:
    intfile(x, file);
    break;
  case _EXEC_FORM_FILEINT:
    fileint(file, base);
    break;
  default:
    usage(execform, cmd);
    break;
  }
  exit(0);
}

// Prints usage summary and exits
void usage(int execform, char* cmd) {
  switch (execform) {
  case _EXEC_FORM_INTFILE:
    printf("Usage: %s FILENAME\n\n", cmd);
    printf("  Read decimal integer input from the user via STDIN, and output the value as unsigned binary to FILENAME.\n");
    break;
  case _EXEC_FORM_FILEINT:
    printf("Usage: %s FILENAME [BASE]\n\n", cmd);
    printf("  Read FILENAME and display it on STDOUT as a single unsigned integer of base BASE.\n");
    printf("  BASE is optional. Default is base 10.\n");
    break;
  default:
    printf("This command recognizes its function based on the executable file name.\n");
    printf("Your command `%s` was not understood.\n", cmd);
    printf("Please execute this program as `int2file` or `file2int` for correct functionality.\n");
    break;
  }

  exit(0);
}

void intfile(mpz_t x, FILE* outfile) {
  FILE* tempfile = tmpfile();

  // Read STDIN into x
  mpz_inp_str(x, stdin, 0);

  // Write x as a single binary number
  mpz_out_raw(tempfile, x);

  // The call to mpz_out_raw() includes a prefix of 4 bytes
  // in order to make it readable by mpz_in_raw(). Here we
  // write it to tempfile, determine the size of tempfil minus 4,
  // and use this value to get the unsigned raw binary data
  // without a prefix/header.
  unsigned long length = ftell(tempfile) - 4;

  // We need a place to store data from tempfile
  unsigned char holder[length+1];

  // Skip 4 bytes at the beginning of tempfile
  fseek(tempfile, 4, SEEK_SET);

  // Read all remaining data into holder
  fread(&holder, 1, length, tempfile);

  // Write the output file with the data from holder
  fwrite(&holder, 1, length, outfile);

  fclose(outfile);
}

void fileint(FILE* infile, unsigned int base) {
  // x will hold the total
  // xx is a temporary mpz_t integer used during the fread() loop
  // tfs is just the value 256 as a GMP mpz_t integer
  mpz_t x, xx, tfs;
  mpz_init(x);
  mpz_init(xx);
  mpz_init(tfs);
  mpz_set_ui(tfs, (unsigned long int)256);
  // onebyte is used to read one byte at a time from the input
  // file. This is doe to avoid endianness conversion.
  unsigned char onebyte;
  // File read error message
  unsigned char* errmsg;

  // To get started, we read a single byte.
  if (fread(&onebyte, 1, 1, infile) != 1) {
    if (feof(infile)) {
      sprintf(errmsg, "File is empty.");
    } else if (ferror(infile)) {
      sprintf(errmsg, "A file read error occured.");
    } else {
      sprintf(errmsg, "An unknown error occured.");
    }
    fprintf(stderr, "ERROR Unable to read from file: %s", errmsg);
  }

  // Start at 0
  mpz_set_ui(x, 0);
  // Put the first byte into x
  mpz_add_ui(xx, x, (unsigned long int)onebyte);
  mpz_set(x, xx);

  // This loop reads one byte from infile and adds it to x
  // Since x is a complex mpz_t integer and not a simple machine
  // int we cannot simply bitshift "<< 8" before adding onebyte to x,
  // so we multiply x by tfs (256) to accomplish a bit shift equivalent
  // to "x << 8"
  while (fread(&onebyte, 1, 1, infile) == 1) {
    // This acts sort of like "x = x << 8" without regard to the
    // internal structure of a mpz_t integer.
    mpz_mul(xx, x, tfs);
    mpz_set(x, xx);

    // After "bit shifting" x left by 8, we can simply add the value
    // of onebyte into x
    mpz_add_ui(x, x, (unsigned long int)onebyte);
  }

  fprintf(stderr, "Outputting file in base %d\n", base);

  mpz_out_str(stdout, base, x);
}
