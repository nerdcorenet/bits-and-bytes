/*
 * bitcount - A program to count the zeroes and ones comprising
 * an input file, with some additional statistics such as the longest
 * consecutive string of 0s or of 1s, and overall 0:1 ratio for the
 * input file.
 *
 * Copyright (c) 2012-2015 Mike Mallett <mike@nerdcore.net>
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

int main (int argc, char *argv[]) {
  // Usage summary
  if (argc == 1) {
    printf("Usage: %s <filename>\n", argv[0]);
    exit(-1);
  }

  // Open file
  char *filename = argv[1];
  FILE *infile = fopen(filename, "rb");
  if (infile == NULL) {
    printf("Unable to open file %s\n", filename);
    exit(-1);
  }

  // Setup variables
  int c; // Current byte
  int i; // For the 8-bit loop
  unsigned int ones=0, zeroes=0;
  unsigned int str_z=0, str_o=0, str_tmp=0; // For consecutive bit count
  char lastbit = -1; // For consecutive bit count

  // The file loop ...
  while ((c = getc(infile)) != EOF) {
    // Check each bit of the current byte
    for (i=0; i<8; i++) {
      if (c & 0x80) {
	ones++;
	if (lastbit == 0) {
	  if (str_tmp > str_z) str_z = str_tmp;
	  str_tmp = 1;
	} else {
	  str_tmp++;
	  if (str_tmp > str_o) str_o = str_tmp;
	}
	lastbit = 1;
      } else {
	zeroes++;
	if (lastbit == 1) {
	  if (str_tmp > str_o) str_o = str_tmp;
	  str_tmp = 1;
	} else {
	  str_tmp++;
	  if (str_tmp > str_z) str_z = str_tmp;
	}
	lastbit = 0;
      }
      c = c << 1;
    }
  }

  if (lastbit==0 && str_tmp > str_z) str_z = str_tmp;
  else if (lastbit==1 && str_tmp > str_o) str_o = str_tmp;

  // Print infoz and close file
  printf("Bit count for %s:\n\n", filename);
  printf("Zeroes        : %u\n", zeroes);
  printf("Ones          : %u\n", ones);
  if (zeroes > 0 && ones > 0) // Avoid division by 0
    printf("One:Zero ratio: 1:%f\n", (double)zeroes/(double)ones);
  printf("Longest consecutive string of zeroes: %u\n", str_z);
  printf("Longest consecutive string of ones  : %u\n", str_o);

  printf("\n");
  fclose(infile);
  exit(1);
}
