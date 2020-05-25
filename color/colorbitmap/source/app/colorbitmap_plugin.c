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

void Extraction(char *img_path, char *fv_path)
{
    CImage *image=NULL;
    Vetor *lucolor; //feature vector
    double media[3],desvio[3];
    long int tam;

    image = ReadCImage(img_path); //opens input image

    tam = image->C[0]->ncols * image->C[0]->nrows;

    lucolor=LuColor2(image,tam,media,desvio);

    WriteFileVetorBin(lucolor,fv_path,media,desvio);

    DestroyVetor(&lucolor);
    DestroyCImage(&image);

}


double Distance(char *fv1_path, char *fv2_path)
{
    double distance,ma[3],mb[3],da[3],db[3];

    Vetor *lucolor1 = ReadFileVetorBin(fv1_path,ma,da);
    Vetor *lucolor2 = ReadFileVetorBin(fv2_path,mb,db);

    distance = Hamming(lucolor1,lucolor2,ma,mb,da,db);

    DestroyVetor(&lucolor1);
    DestroyVetor(&lucolor2);

    return(distance);
}
