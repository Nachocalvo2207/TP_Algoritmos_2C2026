#ifndef PARTE_1_H_INCLUDED
#define PARTE_1_H_INCLUDED

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

typedef struct sNodo {
    void *info;
    unsigned tam_info;
    struct sNodo *sig;
} tNodo;

typedef tNodo *tLista;

typedef struct {
    tLista lista;
    unsigned cap_max;   /// Para chequear cache
    unsigned cap_act;   /// Para chequear cache
    tCmp cmp;          /// Lo dice el enunciado en la parte de crear_lrucache
} t_lru_cache;

// Estructura de prueba (simulando los posteos de la Parte 3 o un producto)
typedef struct {
    int id;
    char descripcion[30];
} t_item;

int crear_lrucache_P1(t_lru_cache *cache, unsigned cap_max, tCmp cmp);
int agregar_lrucache_P1(t_lru_cache *cache, const void *dato, unsigned cantBytes);
int obtener_lrucache_P1(t_lru_cache *cache, void *dato, unsigned cantBytes);
int borrar_lrucache_P1(t_lru_cache *cache, const void *clave);
void vaciar_lrucache_P1(t_lru_cache *cache);
///PARA PROBAR EL MAIN
void mostrar_cache(const t_lru_cache *cache);

void sacar_ultimo_lista(tLista* p); ///Agregada por Nacho
int comparar_items(const void *a, const void *b); ///AGREGADA PARA PODER COMPARAR EN EL MAIN



#endif // PARTE_1_H_INCLUDED
