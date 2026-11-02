#include "arbol_bin_busq.h"

static tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d,
                                           int (*cmp)(const void *, const void *));
static tNodoArbol **mayorNodoArbolBinBusq(const tArbolBinBusq *p);
static tNodoArbol **menorNodoArbolBinBusq(const tArbolBinBusq *p);


void crearArbolBinBusq(tArbolBinBusq *p)
{
    *p = NULL;
}


int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam,
                         int (*cmp)(const void *, const void *))
{
    tNodoArbol *nue;
    int         rc;

    while(*p)
    {
        if((rc = cmp(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if(rc > 0)
            p = &(*p)->der;
        else
            return CLA_DUP;
    }
    nue = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if(!nue || !(nue->info = malloc(tam)))
    {
        free(nue);
        return SIN_MEM;
    }
    nue->tamInfo = tam;
    memcpy(nue->info, d, tam);
    nue->der = nue->izq = NULL;
    *p = nue;
    return TODO_BIEN;
}


int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned tam,
                           int (*cmp)(const void *, const void *))
{
    if(!(p = buscarNodoArbolBinBusq(p, d, cmp)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return 1;
}


int eliminarElemArbolBinBusq(tArbolBinBusq *p, void *d, unsigned tam,
                             int (*cmp)(const void *, const void *))
{
    if(!(p = buscarNodoArbolBinBusq(p, d, cmp)))
        return 0;
    memcpy(d, (*p)->info, MINIMO(tam, (*p)->tamInfo));
    return eliminarRaizArbolBinBusq(p);
}


int eliminarRaizArbolBinBusq(tArbolBinBusq *p)
{
    tNodoArbol **remp,
               *elim;

    if(!*p)
        return 0;

    free((*p)->info);
    if(!(*p)->izq && !(*p)->der)
    {
        free(*p);
        *p = NULL;
        return 1;
    }

    remp = (*p)->der ? menorNodoArbolBinBusq(&(*p)->der)
                     : mayorNodoArbolBinBusq(&(*p)->izq);

    elim = *remp;
    (*p)->info = elim->info;
    (*p)->tamInfo = elim->tamInfo;

    *remp = elim->izq ? elim->izq : elim->der;

    free(elim);
    return 1;
}


unsigned alturaArbolBin(const tArbolBinBusq *p)
{
    unsigned hi,
             hd;
    if(!*p)
        return 0;
    hi = alturaArbolBin(&(*p)->izq);
    hd = alturaArbolBin(&(*p)->der);
    return (hi > hd ? hi : hd) + 1;
}


unsigned cantNodosArbolBin(const tArbolBinBusq *p)
{
    if(!*p)
        return 0;
    return cantNodosArbolBin(&(*p)->izq)
         + cantNodosArbolBin(&(*p)->der)
         + 1;
}


unsigned vaciarArbolBinBusq(tArbolBinBusq *p)
{
    unsigned cant;
    if(!*p)
        return 0;
    cant = vaciarArbolBinBusq(&(*p)->izq) + vaciarArbolBinBusq(&(*p)->der);
    free((*p)->info);
    free(*p);
    *p = NULL;
    return cant + 1;
}


static tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d,
                                           int (*cmp)(const void *, const void *))
{
    int rc;
    while(*p && (rc = cmp(d, (*p)->info)))
    {
        if(rc < 0)
            p = &(*p)->izq;
        else
            p = &(*p)->der;
    }
    if(!*p)
        return NULL;
    return (tNodoArbol **)p;
}


static tNodoArbol **mayorNodoArbolBinBusq(const tArbolBinBusq *p)
{
    if(!*p)
        return NULL;
    while((*p)->der)
        p = &(*p)->der;
    return (tNodoArbol **)p;
}


static tNodoArbol **menorNodoArbolBinBusq(const tArbolBinBusq *p)
{
    if(!*p)
        return NULL;
    while((*p)->izq)
        p = &(*p)->izq;
    return (tNodoArbol **)p;
}
