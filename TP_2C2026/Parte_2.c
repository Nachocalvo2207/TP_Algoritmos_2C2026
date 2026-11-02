#include "Parte_2.h"
#include "Parte_1.h"

void crear_arbol_P2(tArbol *raiz);
void vaciar_arbol_P2(tArbol *raiz);
int insertar_arbol_P2(tArbol *raiz, tNodo_P2 *nodo_lista, tCmp cmp);
tArbol* buscar_arbol_P2(tArbol *raiz, const void *clave, tCmp cmp);
int eliminar_nodo_arbol(tArbol *raiz);
unsigned altura_arbol_P2(tArbol *raiz);

/// Saca 'nodo' de la lista doble SIN liberarlo, arreglando los punteros
/// de sus vecinos y, si corresponde, 'primero' y 'ultimo' de la cache.
void desenganchar_nodo_P2(t_lru_cache_P2 *cache, tNodo_P2 *nodo)
{
    if(nodo->ant)               /// Tiene anterior: el anterior ahora apunta al siguiente
        nodo->ant->sig = nodo->sig;
    else                        /// Era el primero: el nuevo primero es el siguiente
        cache->primero = nodo->sig;

    if(nodo->sig)               /// Tiene siguiente: el siguiente ahora apunta al anterior
        nodo->sig->ant = nodo->ant;
    else                        /// Era el ultimo: el nuevo ultimo es el anterior
        cache->ultimo = nodo->ant;

    nodo->ant = NULL;
    nodo->sig = NULL;
}

/// Engancha 'nodo' (ya desenganchado o nuevo) al principio de la lista.
void enganchar_al_frente_P2(t_lru_cache_P2 *cache, tNodo_P2 *nodo)
{
    nodo->ant = NULL;
    nodo->sig = cache->primero;

    if(cache->primero)
        cache->primero->ant = nodo;
    else                        /// La lista estaba vacia: el nodo tambien es el ultimo
        cache->ultimo = nodo;

    cache->primero = nodo;
}

/// Mueve un nodo que ya esta en la lista al principio (si no lo estaba ya)
void mover_al_frente_P2(t_lru_cache_P2 *cache, tNodo_P2 *nodo)
{
    if(nodo == cache->primero)
        return;

    desenganchar_nodo_P2(cache, nodo);
    enganchar_al_frente_P2(cache, nodo);
}

/// Elimina el ultimo nodo (el menos usado recientemente) gracias al puntero 'ultimo',
/// sin recorrer la lista. Libera el dato y el nodo.
void sacar_ultimo_P2(t_lru_cache_P2 *cache)
{
    tNodo_P2 *aux = cache->ultimo;
    if(!aux)
        return;

    desenganchar_nodo_P2(cache, aux);
    free(aux->info);
    free(aux);
    cache->cap_act--;
}

/// Busca por clave recorriendo la lista desde el principio.
/// Devuelve el nodo encontrado o NULL si la clave no esta.
static tNodo_P2 *buscar_nodo_P2(const t_lru_cache_P2 *cache, const void *clave)
{
    tNodo_P2 *act = cache->primero;

    while(act && cache->cmp(act->info, clave) != 0)
        act = act->sig;

    return act;
}

// Primitivas del TDA
int crear_lrucache_P2(t_lru_cache_P2 *cache, unsigned cap_max, tCmp cmp)
{
    if(cap_max == 0 || cmp == NULL)
        return ERROR;

    cache->primero = NULL;
    cache->ultimo = NULL;
    cache->cap_max = cap_max;
    cache->cap_act = 0;
    cache->cmp = cmp;

    return TODO_OK;
}

int agregar_lrucache_P2(t_lru_cache_P2 *cache, const void *dato, unsigned cantBytes)
{
    tNodo_P2 *existe = buscar_nodo_P2(cache, dato); ///  se pide antes por si no hay memoria la cache quede igual
    void *info = malloc(cantBytes);
    if(!info)
        return SIN_MEM;
    memcpy(info, dato, cantBytes);

    if(existe)  ///Ya existia la clave: reemplazamos el dato y lo pasamos al frente
    {
        free(existe->info);
        existe->info = info;
        existe->tam_info = cantBytes;

        mover_al_frente_P2(cache, existe);
        return TODO_OK;
    }

    /// La clave del elemento no existia: creamos un nodo nuevo al frente
    tNodo_P2 *nue = (tNodo_P2*)malloc(sizeof(tNodo_P2));
    if(!nue)
    {
        free(info);
        return SIN_MEM;
    }

    nue->info = info;
    nue->tam_info = cantBytes;
    enganchar_al_frente_P2(cache, nue);
    cache->cap_act++;

    if(cache->cap_act > cache->cap_max) /// Si nos pasamos, sacamos el ultimo
        sacar_ultimo_P2(cache);

    return TODO_OK;
}

int obtener_lrucache_P2(t_lru_cache_P2 *cache, void *dato, unsigned cantBytes)
{
    tNodo_P2 *existe = buscar_nodo_P2(cache, dato);

    if(!existe)
        return NO_EXISTE;

    /// Devolvemos el valor (sin pasarnos del tamanio de ninguno de los dos buffers)
    memcpy(dato, existe->info, MINIMO(cantBytes, existe->tam_info));

    ///Movemos al principio como pide el TP
    mover_al_frente_P2(cache, existe);

    return ENCONTRADO;
}

int borrar_lrucache_P2(t_lru_cache_P2 *cache, const void *clave)
{
    tNodo_P2 *existe = buscar_nodo_P2(cache, clave);

    if(!existe)
        return NO_EXISTE;

    desenganchar_nodo_P2(cache, existe);
    free(existe->info);
    free(existe);
    cache->cap_act--;

    return TODO_OK;
}

void vaciar_lrucache_P2(t_lru_cache_P2 *cache)
{
    while(cache->primero)
    {
        tNodo_P2 *aux = cache->primero;
        cache->primero = aux->sig;
        free(aux->info);
        free(aux);
    }

    cache->ultimo = NULL;
    cache->cap_act = 0;
}

// esto lo hice para probar en main que este todo ok
void mostrar_cache_P2(const t_lru_cache_P2 *cache)
{
    tNodo_P2 *act = cache->primero;
    printf("--- ESTADO CACHE P2 (Cap: %u/%u) [FRENTE -> FONDO] ---\n", cache->cap_act, cache->cap_max);
    if (!act)
    {
        printf("Cache vacia.\n");
    }
    while (act)
    {
        t_item *it = (t_item *)act->info;
        printf("[ID: %d - %s] <-> ", it->id, it->descripcion);
        act = act->sig;
    }
    printf("NULL\n----------------------------------------------------\n\n");
}

// de aca para arriba

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
    return ERROR; // Duplicado
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
    if (!(*raiz)->izq && !(*raiz)->der) {  // Es hoja
        free(*raiz);
        *raiz = NULL;
        return TODO_OK;
    }
    tArbol *remp; // Tiene hijos, buscamos reemplazo
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
