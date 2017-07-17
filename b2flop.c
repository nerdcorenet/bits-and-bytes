/*
 * b2flop.c - A program to calculate the number of DOS-formatted 3.5"
 * floppy disks required to store a dataset of N bytes. It shows the
 * total height for a stack of 3mm 3.5" floppy disks to store the
 * data.
 *
 * This program requires the GMP library: http://libgmp.org/
 * 
 * Copyright (c) 2017 Mike Mallett <mike@nerdcore.net>
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
  char *cmd = basename(argv[0]);
  mpz_t bytes, floppies, mm, bpf, mmpf;
  mpz_init_set_ui(bpf, 1457664); // Bytes Per Floppy (DOS-format)
  mpz_init_set_ui(mmpf, 3);      // mm Per Floppy (3.5" format)

  // Process command-line args
  if (argc < 2) {
    usage(cmd);
    exit(-1);
  }

  // Get the input value
  mpz_init(bytes);
  if (mpz_set_str(bytes, *(++argv), 10) != 0) {
    fprintf(stderr, "%s: Invalid number of bytes\n", cmd);
    exit(-2);
  }

  // Number of flopies
  mpz_init(floppies);
  mpz_cdiv_q(floppies, bytes, bpf);

  // Number of mm
  mpz_init(mm);
  mpz_mul(mm, floppies, mmpf);

  mpz_out_str(stdout, 10, bytes);
  printf("b = ");
  mpz_out_str(stdout, 10, floppies);
  printf("floppies = ");
  mpz_out_str(stdout, 10, mm);
  printf("mm\n");

  exit(0);
}

// Prints usage summary and exits
void usage(char* cmd) {
  printf("Usage: %s [BYTES]\n\n", cmd);
  printf("  Converts the number of bytes to a distance representing the height\n");
  printf("  of a stack of DOS-formatted 1.44M floppies required to store it.\n");
}
