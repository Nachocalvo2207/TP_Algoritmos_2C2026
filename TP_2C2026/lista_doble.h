#ifndef LISTA_DOBLE_H_INCLUDED
#define LISTA_DOBLE_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "comun.h"

typedef struct sNodo
{
    void            *info;
    unsigned        tamInfo;
    struct sNodo    *sig,
                    *ant;
} tNodo;

typedef tNodo *tLista;

void   crearLista(tLista *p);
int    listaVacia(const tLista *p);

int    vaciarLista(tLista *p);

tNodo *insertarPrimeroLista(tLista *p, const void *d, unsigned tam);

void   ponerPrimeroLista(tLista *p, tNodo *nodo);

int    eliminarNodoLista(tLista *p, tNodo *nodo, void *d, unsigned tam);

int    modificarNodoLista(tNodo *nodo, const void *d, unsigned tam);

void  *infoNodoLista(const tNodo *nodo);
tNodo *anteriorNodoLista(const tNodo *nodo);

int    recorrerLista(const tLista *p, void *params,
                     void (*accion)(void *info, unsigned tam, void *params));

#endif
