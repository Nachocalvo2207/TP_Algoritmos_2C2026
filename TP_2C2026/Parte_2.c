#include "Parte_2.h"

void crear_arbol_P2(tArbol *raiz);
void vaciar_arbol_P2(tArbol *raiz);
int insertar_arbol_P2(tArbol *raiz, tNodo_P2 *nodo_lista, tCmp cmp);
tArbol* buscar_arbol_P2(tArbol *raiz, const void *clave, tCmp cmp);
int eliminar_nodo_arbol(tArbol *raiz);
unsigned altura_arbol_P2(tArbol *raiz);

int crear_lrucache_P2(t_lru_cache_P2 *cache, unsigned cap_max, tCmp cmp)
{
    if(cap_max <= 0 || cmp == NULL)
        return ERROR;



    return TODO_OK;
}
int agregar_lrucache_P2(t_lru_cache_P2 *cache, const void *dato, unsigned cantBytes)
{
    return TODO_OK;
}
int obtener_lrucache_P2(t_lru_cache_P2 *cache, void *dato, unsigned cantBytes);
int borrar_lrucache_P2(t_lru_cache_P2 *cache, const void *clave);
void vaciar_lrucache_P2(t_lru_cache_P2 *cache);

unsigned altura_lrucache_P2(t_lru_cache_P2 *cache)
{
    return altura_arbol_P2(&cache->raiz);
}

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

