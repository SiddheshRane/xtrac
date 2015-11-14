/* 
 * Copyright (C) 2015 Siddhesh
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#define OP 0
#define VERSION 1
#define VERBOSE 1

static char* map[256];
static int tokenCount = 0;

int dextr_main(FILE* input, FILE* output, unsigned flag);
void printMap();

long filesize;

static void load_map(FILE* input) {
    if (!input || feof(input)) {
        return;
    }
    if (fgetc(input)) {
        //if not zero, no map is defined in input file
        fseek(input, -1, SEEK_CUR);
        return;
    }

    int key;
    size_t len;
    while ((key = fgetc(input)) != EOF && key) {
        char* value = NULL;
        getdelim(&value, &len, OP, input);
        map[key] = value;
    }
}

void decompress(FILE* input, FILE* output) {
    if (!input || !output) {
        return;
    }
    int buf;
    while ((buf = fgetc(input)) != EOF) {
        if (map[buf])
            fprintf(output, "%s", map[buf]);
        else
            fputc(buf, output);
    }
}

int dextr_main(FILE* input, FILE* output, unsigned flag) {
    if (!input || !output) {
        return 1;
    }
    fseek(input, 0, 0);
    int buf = fgetc(input);
    if (buf != VERSION)
        return 2;
    load_map(input);
    if (flag & VERBOSE) {
        printMap();
    }
    decompress(input, output);
    fclose(input);
    fclose(output);
}

void printMap() {
    printf("Printing Map\n");
    int i;
    for (i = 0; i < 256; i++) {
        if (map[i]) {
            printf("%3d:%s\n", i, map[i]);
        }
    }

}
