#ifndef LRU_CACHE_H_INCLUDED
#define LRU_CACHE_H_INCLUDED

#include "lista_doble.h"
#include "arbol_bin_busq.h"

#define LRU_OK           1
#define LRU_NO_EXISTE    0
#define LRU_SIN_MEM     -1
#define LRU_ERROR       -2

typedef struct
{
    tLista          lista;
    tLista          ultimo;
    tArbolBinBusq   indice;
    unsigned        capacidad;
    unsigned        cantidad;
    int           (*cmp)(const void *, const void *);
} t_lru_cache;

typedef struct
{
    void        *clave;
    unsigned    tamClave;
    void        *valor;
    unsigned    tamValor;
} tEntradaCache;

typedef struct
{
    tNodo       *nodo;
    const void  *clave;
    int        (*cmp)(const void *, const void *);
} tBusquedaIndice;

int      crear_lrucache(t_lru_cache *c, unsigned capacidad,
                        int (*cmp)(const void *, const void *));

int      agregar_lrucache(t_lru_cache *c, const void *clave, unsigned tamClave,
                          const void *valor, unsigned tamValor);

int      obtener_lrucache(t_lru_cache *c, const void *clave,
                          void *valor, unsigned tamValor);

int      borrar_lrucache(t_lru_cache *c, const void *clave);

void     vaciar_lrucache(t_lru_cache *c);

unsigned altura_indice_lrucache(const t_lru_cache *c);

unsigned cantidad_lrucache(const t_lru_cache *c);

void     recorrer_lrucache(const t_lru_cache *c, void *params,
                           void (*accion)(const void *clave, const void *valor, void *params));

#endif
