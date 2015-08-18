/*
 * bitcat - A program to print the bits (not just the bytes)
 * of an input file.
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

void usage(char*);

int main (int argc, char *argv[]) {
  // Setup variables
  int c; // Current byte
  int i; // For the 8-bit loop
  int cols=0; // For column printing
  char *filename;
  char mout=0; // "Morse Code" output (default: 01)
  char sout=0; // Streaming output (no columns)
  unsigned char argt; // argv tmp var
  char *cmd = argv[0];

  // Process command-line args
  while (--argc > 0 && (*++argv)[0] == '-')
    while (argt = *++argv[0])
      switch (argt) {
      case 'm':
	mout = 1;
	break;
      case 's':
	sout = 1;
	break;
      default:
	usage(cmd);
	break;
      }
  if (argc != 1) {
    usage(cmd);
  }

  // Open file
  filename = *argv;
  FILE *infile = fopen(filename, "rb");
  if (infile == NULL) {
    printf("Unable to open file %s\n", filename);
    exit(-1);
  }

  // The file loop ...
  while ((c = getc(infile)) != EOF) {
    // Check the high-order bit
    for (i=0; i<8; i++) {
      if (c & 0x80)
	printf("%c", (mout)?'-':'1');
      else
	printf("%c", (mout)?'.':'0');
      c = c << 1;
    }
    if (!sout) {
      cols++;
      printf(" ");
      if (cols > 7) {
	printf("\n");
	cols=0;
      }
    }
  }

  printf("\n");
  fclose(infile);
  exit(1);
}

// Prints usage summary and exits
// *cmd is meant to be argv[0]
void usage(char *cmd) {
  printf("Usage: %s [-m] [-s] <filename>\n\n", cmd);
  printf("  -m Prints Morse Code style output ('.' and '-') DEFAULT: 01\n");
  printf("  -s Prints streaming output DEFAULT: 8-byte columns\n");
  exit(-1);
}
