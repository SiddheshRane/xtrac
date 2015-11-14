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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMPRESS 0
#define UNCOMPRESS 1

/*The input and output files*/


void printHelp() {
    printf("Usage:\nxtrac <inputfile> <outputfile>");
}

char* stripExtensionName(char *input, char *ext) {
    if (!input || !ext) {
        return NULL;
    }
    int inputLen = strlen(input);
    int extLen = strlen(ext);
    if (strcmp(input + inputLen - extLen, ext) == 0) {
        char *output = malloc(inputLen - extLen + 1);
        strncpy(output, input, inputLen - extLen);
        output[inputLen - extLen] = '\0';
        return output;
    }
    return input;
}

char* appendExtensionName(char *input, char *ext) {
    int inputLen = strlen(input);
    int extLen = strlen(ext);
    char *output = malloc(inputLen + extLen + 2);
    strcpy(output, input);
    output[inputLen] = '.';
    strcpy(output + inputLen + 1, ext);
    return output;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printHelp();
        return 1;
    }
    FILE *ifile = NULL, *ofile = NULL;
    char *input = NULL, *output = NULL;
    int operation = COMPRESS;

    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-x") == 0) {
            operation = UNCOMPRESS;
        } else {
            input = argv[i];
        }
    }
    if (!input) {
        printf("No input file specified\n");
        return 1;
    }
    if (!output) {
        if (operation == COMPRESS) {
            output = appendExtensionName(input, "xtr");
        } else {
            output = stripExtensionName(input, ".xtr"); //xtrac.c.xtr
            printf("output : %s\n", output);
        }
    }
    if (strcmp(input, output) == 0) {
        printf("Output file name cannot be the same as input file\n");
        return 1;
    }
    ifile = fopen(input, "r");
    if (!ifile) {
        printf("Could not open file %s\n", input);
        return 1;
    }

    ofile = fopen(output, "w");
    if (!ofile) {
        printf("Could not create output file %s\n", output);
        return 1;
    }
    switch (operation) {
        case COMPRESS: xtrac_main(ifile, ofile);
            break;
        case UNCOMPRESS: dextr(ifile, ofile);
            break;
    }
    return 0;
}

