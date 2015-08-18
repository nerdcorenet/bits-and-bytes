/*
 * bwrite - A program to write a single byte into a file. This program
 * requests the user to input a decimal value for the byte to be written
 * in the range 0-255. It writes the appropriate value into the file,
 * or appends it to the end is the '-a' option is given.
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

void usage(char*);

int main (int argc, char *argv[]) {
  // Setup variables
  unsigned char *filename, *filemode;
  unsigned short byte;
  FILE *outfile;
  char a_out=0; // Append to file
  unsigned char argt; // argv tmp var
  char *cmd = argv[0];

  // Process command-line args
  while (--argc > 0 && (*++argv)[0] == '-')
    while (argt = *++argv[0])
      switch (argt) {
      case 'a':
	a_out = 1;
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
  
  (a_out) ? (filemode = "ab") : (filemode = "wb");
  outfile = fopen(filename, filemode);
  if (outfile == NULL) {
    printf("Unable to open file %s\n", filename);
    exit(-1);
  }

  printf("Byte to write: ");
  scanf("%hu",&byte);
  putc(byte,outfile);
  fclose(outfile);
  exit(1);
}

// Prints usage summary and exits
// *cmd is meant to be argv[0]
void usage(char *cmd) {
  printf("Byte Write\n\n");
  printf("Usage: %s [-a] <filename>\n\n", cmd);
  printf("  -a Append to file (DEFAULT: Overwrite)\n");
  exit(-1);
}
