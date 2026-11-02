#include "lru_cache.h"

typedef struct
{
    unsigned tamClave;
    unsigned tamValor;
} tEntradaCache;

#define ALINEACION          16
#define REDONDEAR(n)        (((n) + ALINEACION - 1) / ALINEACION * ALINEACION)
#define DESP_CLAVE          REDONDEAR(sizeof(tEntradaCache))
#define CLAVE(e)            ((char *)(e) + DESP_CLAVE)
#define VALOR(e)            (CLAVE(e) + REDONDEAR(((tEntradaCache *)(e))->tamClave))
#define TAM_ENTRADA(tc, tv) (DESP_CLAVE + REDONDEAR(tc) + (tv))

typedef struct
{
    tNodo       *nodo;
    const void  *clave;
    int        (*cmp)(const void *, const void *);
} tBusquedaIndice;

typedef struct
{
    void (*accion)(const void *clave, const void *valor, void *params);
    void  *params;
} tRecorridoCache;


static int cmpIndice(const void *d, const void *info)
{
    const tBusquedaIndice *b    = (const tBusquedaIndice *)d;
    const tNodo           *nodo = *(tNodo * const *)info;

    return b->cmp(b->clave, CLAVE(infoNodoLista(nodo)));
}

static tNodo *buscarEnIndice(const t_lru_cache *c, const void *clave)
{
    tBusquedaIndice b;

    b.nodo  = NULL;
    b.clave = clave;
    b.cmp   = c->cmp;
    if(!buscarElemArbolBinBusq(&c->indice, &b, sizeof(b.nodo), cmpIndice))
        return NULL;
    return b.nodo;
}

static tEntradaCache *armarEntrada(const void *clave, unsigned tamClave,
                                   const void *valor, unsigned tamValor)
{
    tEntradaCache *e = (tEntradaCache *)malloc(TAM_ENTRADA(tamClave, tamValor));

    if(!e)
        return NULL;
    e->tamClave = tamClave;
    e->tamValor = tamValor;
    memcpy(CLAVE(e), clave, tamClave);
    memcpy(VALOR(e), valor, tamValor);
    return e;
}

static void ponerPrimero(t_lru_cache *c, tNodo *nodo)
{
    tNodo *ant = anteriorNodoLista(nodo);

    if(nodo == c->ultimo && ant)
        c->ultimo = ant;
    ponerPrimeroLista(&c->lista, nodo);
}

static void sacarDeLista(t_lru_cache *c, tNodo *nodo)
{
    if(nodo == c->ultimo)
        c->ultimo = anteriorNodoLista(nodo);
    eliminarNodoLista(&c->lista, nodo, NULL, 0);
    c->cantidad--;
}

static void descartarUltimo(t_lru_cache *c)
{
    tNodo           *nodo = c->ultimo;
    tBusquedaIndice b;

    b.nodo  = NULL;
    b.clave = CLAVE(infoNodoLista(nodo));
    b.cmp   = c->cmp;
    eliminarElemArbolBinBusq(&c->indice, &b, sizeof(b.nodo), cmpIndice);

    sacarDeLista(c, nodo);
}

static void accionEntrada(void *info, unsigned tam, void *params)
{
    tRecorridoCache *r = (tRecorridoCache *)params;

    (void)tam;
    r->accion(CLAVE(info), VALOR(info), r->params);
}


int crear_lrucache(t_lru_cache *c, unsigned capacidad,
                   int (*cmp)(const void *, const void *))
{
    if(!c || !capacidad || !cmp)
        return LRU_ERROR;
    crearLista(&c->lista);
    c->ultimo = NULL;
    crearArbolBinBusq(&c->indice);
    c->capacidad = capacidad;
    c->cantidad  = 0;
    c->cmp       = cmp;
    return LRU_OK;
}


int agregar_lrucache(t_lru_cache *c, const void *clave, unsigned tamClave,
                     const void *valor, unsigned tamValor)
{
    tNodo           *nodo;
    tEntradaCache   *e;
    tBusquedaIndice b;
    unsigned        tam = TAM_ENTRADA(tamClave, tamValor);
    int             r;

    if(!c || !clave || !valor || !tamClave || !tamValor)
        return LRU_ERROR;
    if(!(e = armarEntrada(clave, tamClave, valor, tamValor)))
        return LRU_SIN_MEM;

    if((nodo = buscarEnIndice(c, clave)) != NULL)
    {
        ponerPrimero(c, nodo);
        r = modificarNodoLista(nodo, e, tam);
        free(e);
        return r == TODO_BIEN ? LRU_OK : LRU_SIN_MEM;
    }

    if(c->cantidad == c->capacidad)
        descartarUltimo(c);

    nodo = insertarPrimeroLista(&c->lista, e, tam);
    free(e);
    if(!nodo)
        return LRU_SIN_MEM;
    if(!c->ultimo)
        c->ultimo = nodo;
    c->cantidad++;

    b.nodo  = nodo;
    b.clave = CLAVE(infoNodoLista(nodo));
    b.cmp   = c->cmp;
    if(insertarArbolBinBusq(&c->indice, &b, sizeof(b.nodo), cmpIndice) != TODO_BIEN)
    {
        sacarDeLista(c, nodo);
        return LRU_SIN_MEM;
    }
    return LRU_OK;
}


int obtener_lrucache(t_lru_cache *c, const void *clave, void *valor, unsigned tamValor)
{
    tNodo           *nodo;
    tEntradaCache   *e;

    if(!c || !clave)
        return LRU_ERROR;
    if(!(nodo = buscarEnIndice(c, clave)))
        return LRU_NO_EXISTE;

    ponerPrimero(c, nodo);

    e = (tEntradaCache *)infoNodoLista(nodo);
    if(valor)
        memcpy(valor, VALOR(e), MINIMO(tamValor, e->tamValor));
    return LRU_OK;
}


int borrar_lrucache(t_lru_cache *c, const void *clave)
{
    tBusquedaIndice b;

    if(!c || !clave)
        return LRU_ERROR;

    b.nodo  = NULL;
    b.clave = clave;
    b.cmp   = c->cmp;
    if(!eliminarElemArbolBinBusq(&c->indice, &b, sizeof(b.nodo), cmpIndice))
        return LRU_NO_EXISTE;

    sacarDeLista(c, b.nodo);
    return LRU_OK;
}


void vaciar_lrucache(t_lru_cache *c)
{
    if(!c)
        return;
    vaciarArbolBinBusq(&c->indice);   
    vaciarLista(&c->lista);           
    c->cantidad = 0;
}


unsigned altura_indice_lrucache(const t_lru_cache *c)
{
    return alturaArbolBin(&c->indice);
}


unsigned cantidad_lrucache(const t_lru_cache *c)
{
    return c->cantidad;
}


void recorrer_lrucache(const t_lru_cache *c, void *params,
                       void (*accion)(const void *clave, const void *valor, void *params))
{
    tRecorridoCache r;

    r.accion = accion;
    r.params = params;
    recorrerLista(&c->lista, &r, accionEntrada);
}
