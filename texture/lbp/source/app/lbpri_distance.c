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
#include "libdescriptors.h"

double Distance(char *fv1_path, char *fv2_path)
{
    Histogram *lbp1=NULL;
    Histogram *lbp2=NULL;
    double distance;

    lbp1 = ReadFileHistogram(fv1_path);
    lbp2 = ReadFileHistogram(fv2_path);

    distance=L1Distance(lbp1, lbp2);

    DestroyHistogram(&lbp1);
    DestroyHistogram(&lbp2);

    return (distance);
}

int main(int argc, char** argv)
{
    double distance;

    if (argc != 3) {
        fprintf(stderr,"Usage: lbpri_distance <fv1_path> <fv2_path>\n");
        fprintf(stderr,"\tfv_path: path to the feature vector files corresponding to the images from which the user wants to compute the distance\n");
        fprintf(stderr,"Remark:\n\tThis program computes the distance between 2 images.\n");
        fprintf(stderr,"\tThe arguments are files generated by the 'extraction' program.\n");
        fprintf(stderr,"\tThe distance between the two images is printed on stdout.\n");
        exit(-1);
    }

    distance=Distance(argv[1], argv[2]);
    printf("%lf\n",distance);

    return(0);
}
