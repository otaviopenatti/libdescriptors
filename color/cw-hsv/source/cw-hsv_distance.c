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

#define STR_SIZE 63

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr,"Usage: cw-hsv_distance <fv1_path> <fv2_path>\n");
        fprintf(stderr,"\tfv_path: path to the feature vector files corresponding to the images from which the user wants to compute the distance\n");
        fprintf(stderr,"Remark:\n\tThis program computes the distance between 2 images.\n");
        fprintf(stderr,"\tThe arguments are files generated by the 'extraction' program.\n");
        fprintf(stderr,"\tThe distance between the two images is printed on stdout.\n");
        exit(-1);
    }

    char *fv1_path = argv[1];
    char *fv2_path = argv[2];
    // unsigned char str1[STR_SIZE], str2[STR_SIZE];
    unsigned long long int num1, num2, resultado;
    int num;
    int i, dist=0;

    //Opening input files
    FILE *f1, *f2;
    if ((f1 = fopen(fv1_path, "r")) == NULL) {
        printf("Error opening file: %s\n", fv1_path);
        exit(0);
    }
    if ((f2 = fopen(fv2_path, "r")) == NULL) {
        printf("Error opening file: %s\n", fv2_path);
        exit(0);
    }

    //Reads fv1
    num1 = 0;
    for (i=0; i<STR_SIZE; i++) {
        num1 = num1 << 1;
        num = fgetc(f1);
        num1 = num1 | ((int)num-48);
    }
    //Reads fv2
    num2 = 0;
    for (i=0; i<STR_SIZE; i++) {
        num2 = num2 << 1;
        num = fgetc(f2);
        num2 = num2 | ((int)num-48);
    }

    resultado = num1 ^ num2;

    //Counts the number of 1s in the result
    dist = 0;
    while (resultado != 0) {
        dist += (int)(resultado & 1);
        resultado = resultado >> 1;
    }

    fclose(f1);
    fclose(f2);

    printf("%d\n", dist);
    return 0;
}
