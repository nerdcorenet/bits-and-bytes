/*
 * bytecount - A program to collect and display some byte-level data
 * heiruistics about an input file, such as number of occurances of
 * each byte (0x00 - 0xFF) and number of unique bytes.
 *
 * Copyright (c) 2019 Mike Mallett <mike@nerdcore.net>
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
  int i; // Iterator
  unsigned char uniques = 0; // Number of unique bytes
  char missing[1280] = ""; // " 0xNN"=5 chars; (256 * 5) = 1280
  // We will use this in the iterative loop to count occurances of
  // each possible byte
  unsigned long bytes[256];
  // Initialize the array
  for (i=0; i<256; i++)
    bytes[i] = 0;

  // The file loop ...
  while ((c = getc(infile)) != EOF)
    bytes[c]++;

  // TODO: Sorting?

  // Print infoz and close file
  for (i=0; i<256; i++) {
    if (bytes[i] > 0) {
      uniques++;
      // Only print bytes which exist in the file.
      // We will print a list of "missing" bytes at the end.
      printf("Number of 0x%02Xs: %lu\n", i, bytes[i]);
    } else {
      sprintf(missing, "%s 0x%02X", missing, i);
    }
  }

  printf("\n");
  printf("Number of unique bytes: %d\n", uniques);
  printf("Bytes not appearing in this file: %s\n", missing);

  fclose(infile);
  exit(1);
}
