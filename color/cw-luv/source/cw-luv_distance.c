/*
    COPYRIGHT 2007-2012, 2020

    This file is part of Lib Descriptors.

    Lib Descriptors is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Lib Descriptors is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Lib Descriptors. If not, see <http://www.gnu.org/licenses/>.
    
    Otavio A. B. Penatti - otavio_at_penatti_dot_com
    https://github.com/otaviopenatti/libdescriptors
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

#define STR_SIZE 127

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr,"Usage: cw-luv_distance <fv1_path> <fv2_path>\n");
        fprintf(stderr,"\tfv_path: path to the feature vector files corresponding to the images from which the user wants to compute the distance\n");
        fprintf(stderr,"Remark:\n\tThis program computes the distance between 2 images.\n");
        fprintf(stderr,"\tThe arguments are files generated by the 'extraction' program.\n");
        fprintf(stderr,"\tThe distance between the two images is printed on stdout.\n");
        exit(-1);
    }

    int i, dist=0;

    int *fv1, *fv2;

    fv1 = (int*) calloc(STR_SIZE, sizeof(int));
    fv2 = (int*) calloc(STR_SIZE, sizeof(int));

    //Opening input files
    FILE *f1, *f2;
    if ((f1 = fopen(argv[1], "rb")) == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        exit(0);
    }
    if ((f2 = fopen(argv[2], "rb")) == NULL) {
        printf("Error opening file: %s\n", argv[2]);
        exit(0);
    }

    fread(fv1, sizeof(int), STR_SIZE, f1);
    fread(fv2, sizeof(int), STR_SIZE, f2);
    for (i=0; i<STR_SIZE; i++) {
        dist += (fv1[i] != fv2[i]);
    }
    fclose(f1);
    fclose(f2);
    free(fv1);
    free(fv2);

    printf("%d\n", dist);
    return 0;
}
