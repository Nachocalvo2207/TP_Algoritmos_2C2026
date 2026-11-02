#include "parte_2.h"

//Primitivas para Arbol
void crear_arbol_P2(tArbol *raiz);
void vaciar_arbol_P2(tArbol *raiz);
int insertar_arbol_P2(tArbol *raiz, tNodo_P2 *nodo_lista, tCmp cmp);
tArbol* buscar_arbol_P2(tArbol *raiz, const void *clave, tCmp cmp);
int eliminar_nodo_arbol(tArbol *raiz);
unsigned altura_arbol_P2(tArbol *raiz);

// Auxiliares de Lista Doblemente Enlazada
static void _desvincular_nodo_LDE(t_lru_cache_P2 *cache, tNodo_P2 *nodo)
{
    if (nodo->ant)
        nodo->ant->sig = nodo->sig;
    else
        cache->primero = nodo->sig;

    if (nodo->sig)
        nodo->sig->ant = nodo->ant;
    else
        cache->ultimo = nodo->ant;

    nodo->sig = NULL;
    nodo->ant = NULL;
}

static void _insertar_al_frente_LDE(t_lru_cache_P2 *cache, tNodo_P2 *nodo)
{
    nodo->ant = NULL;
    nodo->sig = cache->primero;
    if (cache->primero)
        cache->primero->ant = nodo;
    else
        cache->ultimo = nodo;
    cache->primero = nodo;
}

int crear_lrucache_P2(t_lru_cache_P2 *cache, unsigned cap_max, tCmp cmp)
{
    if (cap_max == 0 || cmp == NULL)
        return ERROR;

    cache->primero = NULL;
    cache->ultimo = NULL;
    crear_arbol_P2(&cache->raiz);
    cache->cap_max = cap_max;
    cache->cap_act = 0;
    cache->cmp = cmp;

    return TODO_OK;
}

int agregar_lrucache_P2(t_lru_cache_P2 *cache, const void *dato, unsigned cantBytes)
{
    tArbol *p_nodo_arb = buscar_arbol_P2(&cache->raiz, dato, cache->cmp);

    if (p_nodo_arb != NULL) // La clave ya existe en la caché
    {
        tNodo_P2 *nodo_lde = (*p_nodo_arb)->nodo;
        void *nueva_info = malloc(cantBytes);
        if (!nueva_info) return SIN_MEM;

        memcpy(nueva_info, dato, cantBytes);
        free(nodo_lde->info);
        nodo_lde->info = nueva_info;
        nodo_lde->tam_info = cantBytes;

        if (cache->primero != nodo_lde)
        {
            _desvincular_nodo_LDE(cache, nodo_lde);
            _insertar_al_frente_LDE(cache, nodo_lde);
        }
        return TODO_OK;
    }

    // No existe: si la caché está llena, descartamos el último (LRU)
    if (cache->cap_act == cache->cap_max)
    {
        tNodo_P2 *victima = cache->ultimo;
        tArbol *p_arb_victima = buscar_arbol_P2(&cache->raiz, victima->info, cache->cmp);
        if (p_arb_victima)
            eliminar_nodo_arbol(p_arb_victima);

        _desvincular_nodo_LDE(cache, victima);
        free(victima->info);
        free(victima);
        cache->cap_act--;
    }

    // Creamos el nuevo nodo de la lista
    tNodo_P2 *nue = (tNodo_P2 *)malloc(sizeof(tNodo_P2));
    if (!nue) return SIN_MEM;

    nue->info = malloc(cantBytes);
    if (!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, dato, cantBytes);
    nue->tam_info = cantBytes;

    _insertar_al_frente_LDE(cache, nue);

    if (insertar_arbol_P2(&cache->raiz, nue, cache->cmp) != TODO_OK)
    {
        _desvincular_nodo_LDE(cache, nue);
        free(nue->info);
        free(nue);
        return SIN_MEM;
    }

    cache->cap_act++;
    return TODO_OK;
}

int obtener_lrucache_P2(t_lru_cache_P2 *cache, void *dato, unsigned cantBytes)
{
    tArbol *p_nodo_arb = buscar_arbol_P2(&cache->raiz, dato, cache->cmp);
    if (!p_nodo_arb)
        return NO_EXISTE;

    tNodo_P2 *nodo_lde = (*p_nodo_arb)->nodo;
    memcpy(dato, nodo_lde->info, MINIMO(cantBytes, nodo_lde->tam_info));

    if (cache->primero != nodo_lde)
    {
        _desvincular_nodo_LDE(cache, nodo_lde);
        _insertar_al_frente_LDE(cache, nodo_lde);
    }

    return TODO_OK;
}

int borrar_lrucache_P2(t_lru_cache_P2 *cache, const void *clave)
{
    tArbol *p_nodo_arb = buscar_arbol_P2(&cache->raiz, clave, cache->cmp);
    if (!p_nodo_arb)
        return NO_EXISTE;

    tNodo_P2 *nodo_lde = (*p_nodo_arb)->nodo;

    eliminar_nodo_arbol(p_nodo_arb);
    _desvincular_nodo_LDE(cache, nodo_lde);

    free(nodo_lde->info);
    free(nodo_lde);
    cache->cap_act--;

    return TODO_OK;
}

void vaciar_lrucache_P2(t_lru_cache_P2 *cache)
{
    vaciar_arbol_P2(&cache->raiz);

    tNodo_P2 *act = cache->primero;
    while (act)
    {
        tNodo_P2 *aux = act;
        act = act->sig;
        free(aux->info);
        free(aux);
    }

    cache->primero = NULL;
    cache->ultimo = NULL;
    cache->cap_act = 0;
}

unsigned altura_lrucache_P2(t_lru_cache_P2 *cache)
{
    return altura_arbol_P2(&cache->raiz);
}

// Árbol
void crear_arbol_P2(tArbol *raiz){
    *raiz = NULL;
}

void vaciar_arbol_P2(tArbol *raiz){
    if (!*raiz) return;
    vaciar_arbol_P2(&(*raiz)->izq);
    vaciar_arbol_P2(&(*raiz)->der);
    free(*raiz);
    *raiz = NULL;
}

int insertar_arbol_P2(tArbol *raiz, tNodo_P2 *nodo_lista, tCmp cmp){
    if (!*raiz) {
        *raiz = (tArbol)malloc(sizeof(tNodo_arbol_P2));
        if (!*raiz) return SIN_MEM;
        (*raiz)->nodo = nodo_lista;
        (*raiz)->izq = NULL;
        (*raiz)->der = NULL;
        return TODO_OK;
    }
    int res = cmp(nodo_lista->info, (*raiz)->nodo->info);
    if (res < 0) return insertar_arbol_P2(&(*raiz)->izq, nodo_lista, cmp);
    if (res > 0) return insertar_arbol_P2(&(*raiz)->der, nodo_lista, cmp);
    return ERROR;
}

tArbol* buscar_arbol_P2(tArbol *raiz, const void *clave, tCmp cmp){
    if (!*raiz) return NULL;
    int res = cmp(clave, (*raiz)->nodo->info);
    if (res == 0) return raiz;
    if (res < 0) return buscar_arbol_P2(&(*raiz)->izq, clave, cmp);
    return buscar_arbol_P2(&(*raiz)->der, clave, cmp);
}

int eliminar_nodo_arbol(tArbol *raiz){
    if (!*raiz) return NO_EXISTE;
    if (!(*raiz)->izq && !(*raiz)->der) { // Es hoja
        free(*raiz);
        *raiz = NULL;
        return TODO_OK;
    }
    tArbol *remp;  // Tiene hijos, buscamos reemplazo
    if (altura_arbol_P2(&(*raiz)->izq) > altura_arbol_P2(&(*raiz)->der)) {
        remp = &(*raiz)->izq;
        while ((*remp)->der) remp = &(*remp)->der;
    } else {
        remp = &(*raiz)->der;
        while ((*remp)->izq) remp = &(*remp)->izq;
    }
    tArbol elim = *remp;
    (*raiz)->nodo = elim->nodo; // Se transfiere la referencia al nodo de la lista
    *remp = elim->izq ? elim->izq : elim->der;
    free(elim);
    return TODO_OK;
}

// primitiva que informe la altura del árbol índice
unsigned altura_arbol_P2(tArbol *raiz){
    if (!*raiz) return 0;
    unsigned alt_izq = altura_arbol_P2(&(*raiz)->izq);
    unsigned alt_der = altura_arbol_P2(&(*raiz)->der);
    return (alt_izq > alt_der ? alt_izq : alt_der) + 1;
}
