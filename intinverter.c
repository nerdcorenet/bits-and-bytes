/*
 * intinverter - Prints hex and binary of an int, and the hex and
 * binary of the negative of that int (i.e. "0-n")
 *
 * Copyright (c) 2024 Mike Mallett <mike@nerdcore.net>
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

#include <stdlib.h>
#include <stdio.h>

void show_int(int);

int main() {
  printf("Type a number: ");
  char str[20];
  fgets(str, 20, stdin);
  int i = atoi(str);  show_int(i);
  printf("-%d:\n", i);
  show_int(0-i);
}

void show_int(int i) {
  printf("Hex Digits:\n");
  printf("0x%x\n",i);
  printf("Binary Walk:\n");
  for (int byte=0; byte<(sizeof(i)*8); byte++) {
    if (i & 1)
      printf("1");
    else
      printf("0");
    i = i >> 1;
  }
  printf("\n");
}
