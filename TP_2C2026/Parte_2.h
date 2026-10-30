#ifndef PARTE_2_H_INCLUDED
#define PARTE_2_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TODO_OK 1
#define ENCONTRADO 1
#define ERROR 0
#define NO_EXISTE -1
#define SIN_MEM -2
#define MINIMO(X, Y) (((X) <= (Y)) ? (X) : (Y))

typedef int (*tCmp)(const void *a, const void *b); ///Chequear si esto esta bien... costumbre de PAN

typedef struct sNodo_P2 {
    void *info;
    unsigned tam_info;
    struct sNodo_P2 *sig;
    struct sNodo_P2 *ant;
} tNodo_P2;

/// Nodo del ABB: NO tiene el dato, solo apunta al nodo de la lista que lo tiene
typedef struct sNodo_arbol_P2 {
    tNodo_P2 *nodo;
    struct sNodo_arbol_P2 *izq;
    struct sNodo_arbol_P2 *der;
} tNodo_arbol_P2;

typedef struct {
    tNodo_P2 *primero;   /// Usado mas recientemente
    tNodo_P2 *ultimo;    /// Menos usado recientemente
    tNodo_arbol_P2 *raiz;        /// Indice por clave
    unsigned cap_max;
    unsigned cap_act;
    tCmp cmp;
} t_lru_cache_P2;


typedef tNodo_P2 *tLista;
int crear_lrucache_P2(t_lru_cache_P2 *cache, unsigned cap_max, tCmp cmp);
int agregar_lrucache_P2(t_lru_cache_P2 *cache, const void *dato, unsigned cantBytes);
int obtener_lrucache_P2(t_lru_cache_P2 *cache, void *dato, unsigned cantBytes);
int borrar_lrucache_P2(t_lru_cache_P2 *cache, const void *clave);
void vaciar_lrucache_P2(t_lru_cache_P2 *cache);


#endif // PARTE_2_H_INCLUDED
