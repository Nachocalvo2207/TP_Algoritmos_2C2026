#ifndef ARBOL_BIN_BUSQ_H_INCLUDED
#define ARBOL_BIN_BUSQ_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "comun.h"

typedef struct sNodoArbol
{
    void                *info;
    unsigned            tamInfo;
    struct sNodoArbol   *izq,
                        *der;
} tNodoArbol;

typedef tNodoArbol *tArbolBinBusq;

void     crearArbolBinBusq(tArbolBinBusq *p);

int      insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam,
                              int (*cmp)(const void *, const void *));

int      buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned tam,
                                int (*cmp)(const void *, const void *));

int      eliminarElemArbolBinBusq(tArbolBinBusq *p, void *d, unsigned tam,
                                  int (*cmp)(const void *, const void *));

int      eliminarRaizArbolBinBusq(tArbolBinBusq *p);

unsigned alturaArbolBin(const tArbolBinBusq *p);

unsigned cantNodosArbolBin(const tArbolBinBusq *p);

unsigned vaciarArbolBinBusq(tArbolBinBusq *p);

#endif
