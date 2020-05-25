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
#include "feature.h"

Features* CreateFeatures(int ncols, int nrows, int nfeats)
{
    Features *f=(Features *)calloc(1,sizeof(Features));

    f->ncols  = ncols;
    f->nrows  = nrows;
    f->nelems = ncols*nrows;
    f->elem   = (FElem *)calloc(f->nelems,sizeof(FElem));
    f->nfeats = nfeats;

    int i;

    for (i=0; i < f->nelems; i++)
    {
        f->elem[i].feat = AllocFloatArray(nfeats);
    }

    return f;
}

void DestroyFeatures(Features **f)
{
  int i;

  if ((*f)!=NULL){
    for (i=0; i < (*f)->nelems; i++)
      free((*f)->elem[i].feat);
    free((*f)->elem);
    free(*f);
    (*f)=NULL;
  }
}

DImage* GetFeature(Features* feats, int index)
{
    if (feats == NULL) return NULL;
    if (index >= feats->nfeats) return NULL;

    DImage* imgfeats = CreateDImage(feats->ncols, feats->nrows);

    int i;

    for (i = 0; i < feats->nelems; i++)
    {
        if (feats->elem == NULL || feats->elem[i].feat == NULL)
        {
            DestroyDImage(&imgfeats);
            perror("Error! Pixels or feature with NULL values in GetFeature!");
            return NULL;
        }

        double value = (double)feats->elem[i].feat[index];
        imgfeats->val[i] = value;
    }

    return imgfeats;
}


int SetFeature(Features* feats, int index, DImage* imagefeats)
{

    if (feats == NULL || imagefeats == NULL) return 0;;
    if (index >= feats->nfeats) return 0;

    int i;

    for (i = 0; i < feats->nelems; i++)
    {
        if (feats->elem == NULL || feats->elem[i].feat == NULL)
        {
            fprintf(stderr,"Error! Pixels or feature with NULL values!\n");
            return 0;
        }
        feats->elem[i].feat[index] = (float)imagefeats->val[i];
    }
    return 1;
}

Features* ConcatFeatures(Features* f1, Features* f2)
{
    if (f1 == NULL || f2 == NULL)
    {
        Error("Feature f1 or feature f2 is NULL","function ConcatFeatures");
    }
    if (f1->nelems != f2->nelems)
    {
        Error("Distinct number of elements between "
              "features f1 and f2","function ConcatFeatures");
    }
    if (f1->ncols != f2->ncols || f1->nrows != f2->nrows)
    {
        Error("Number of rows, or columns, is different between"
              " features f1 and f2", "function ConcatFeatures");
    };

    int ncols = f1->ncols;
    int nrows = f1->nrows;
    int nfeats = f1->nfeats + f2->nfeats;

    Features* result = CreateFeatures(ncols, nrows, nfeats);

    int i, j;

    for (i=0; i < f1->nelems; i++)
    {
        for (j = 0; j < nfeats; j++)
        {
            if (j < f1->nfeats)
                result->elem[i].feat[j] = f1->elem[i].feat[j];
            else
                result->elem[i].feat[j] = f2->elem[i].feat[j-f1->nfeats];
        }
    }

    return result;

}

Features* RemoveZeroes(Features* feats,int ncols, int nrows)
{
    if (feats == NULL)
    {
        Error("Null Features", "CutFeats\n");
    }
    if (ncols > feats->ncols || nrows > feats->nrows)
    {
        Error("Ncols/Nrows parameter is greater than  feats->ncols/feats->nrows","CutFeats");
    }

    Features* result = CreateFeatures(ncols,nrows,feats->nfeats);

    int p,t;
    int size = feats->nfeats*sizeof(float);
    int x,y;

    for (y = 0; y < nrows; y++)
    {
        for (x = 0; x < ncols; x++)
        {
            p = x + y*ncols;

            t = x + y*feats->ncols;

            memcpy(result->elem[p].feat,feats->elem[t].feat,size);
        }
    }
    return result;
}


/*normalize features*/
void FNormalizeFeatures(Features *f)
{
    float *mean = (float *)calloc(f->nfeats,sizeof(float)), *std = (float *)calloc(f->nfeats, sizeof(int));
    int i,j;

    for (i = 0; i < f->nfeats; i++)
    {
        for (j = 0; j < f->nelems; j++)
            mean[i]+=f->elem[j].feat[i]/f->nelems;
        for (j = 0; j < f->nelems; j++)
            std[i]+=pow(f->elem[j].feat[i]-mean[i],2)/f->nelems;
        std[i]=sqrt(std[i]);
    }

    for (i = 0; i < f->nfeats; i++)
    {
        for (j = 0; j < f->nelems; j++)
        {
            float v =(f->elem[j].feat[i]-mean[i]);
            /// Preventing division by zero
            if (std[i] != 0.0)
                f->elem[j].feat[i] = v/std[i];
            else
                f->elem[j].feat[i] = v;
        }
    }

    free(mean);
    free(std);
}
