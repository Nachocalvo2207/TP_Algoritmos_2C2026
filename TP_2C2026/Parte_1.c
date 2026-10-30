#include "Parte_1.h"

int crear_lrucache_P1(t_lru_cache *cache, unsigned cap_max, tCmp cmp)
{
    if(cap_max == 0 || cmp == NULL)
        return ERROR;

    cache->lista = NULL;
    cache->cap_max = cap_max;
    cache->cap_act = 0;
    cache->cmp = cmp;

    return TODO_OK;
}

void vaciar_lrucache_P1(t_lru_cache *cache)
{
    while(cache->lista)
    {
        tNodo* aux = cache->lista;
        cache->lista = aux->sig;
        free(aux->info);
        free(aux);
    }

    cache->cap_act = 0;
}

int borrar_lrucache_P1(t_lru_cache *cache, const void *clave)
{
    if(!cache->lista)
    {
        return NO_EXISTE;
    }
    tLista* p = &cache->lista;

    while(*p && cache->cmp((*p)->info,clave) != 0)
        p = &(*p)->sig;

    if(!*p)
    {
        return NO_EXISTE;
    }

    tNodo* aux = *p;
    *p = aux->sig;

    free(aux->info);
    free(aux);

    cache->cap_act--;

    return TODO_OK;
}
int agregar_lrucache_P1(t_lru_cache *cache, const void *dato, unsigned cantBytes)
{
    tLista* p = &cache->lista;

    while(*p && cache->cmp((*p)->info,dato) != 0)
        p = &(*p)->sig;

    if(!*p) /// La clave del elemento no existia...
    {
        tNodo* nue = (tNodo*)malloc(sizeof(tNodo));
        if(!nue)
            return SIN_MEM;

        nue->info = malloc(cantBytes);
        if(!nue->info)
        {
            free(nue);
            return SIN_MEM;
        }

        memcpy(nue->info,dato,cantBytes);
        nue->tam_info = cantBytes;
        nue->sig = cache->lista;
        cache->lista = nue;
        cache->cap_act++;

        if(cache->cap_act > cache->cap_max) /// Si nos pasamos, sacamos el último
        {
            sacar_ultimo_lista(&cache->lista);
            cache->cap_act--;
        }
    }
    else  ///Ya existia la clave
    {
        tNodo* existe = *p;
        void* info = malloc(cantBytes);
        if(!info)
            return SIN_MEM;

        memcpy(info,dato,cantBytes);
        free(existe->info); /// Saco lo anterior
        existe->info = info;/// Dejo lo nuevo
        existe->tam_info = cantBytes;

        ///Movemos al principio como pide el TP
        if(*p != cache->lista)
        {
            *p = existe->sig;
            existe->sig = cache->lista;
            cache->lista = existe;
        }


    }

    return TODO_OK;
}

int obtener_lrucache_P1(t_lru_cache *cache, void *dato, unsigned tamBytes)
{
    tLista *p = &cache->lista;

    while(*p && cache->cmp((*p)->info, dato) != 0)
    {
        p = &(*p)->sig;
    }

    if(!*p)
        return NO_EXISTE;

    tNodo *existe = *p;
    memcpy(dato, existe->info, MINIMO(tamBytes, existe->tam_info));


    ///Movemos al principio como pide el TP
    if(p != &cache->lista)
    {
        *p = existe->sig;
        existe->sig = cache->lista;
        cache->lista = existe;
    }

    return TODO_OK;
}

int comparar_items(const void *a, const void *b)
{
    const t_item *item1 = (const t_item *)a;
    const t_item *item2 = (const t_item *)b;
    return item1->id - item2->id;
}


///PARA PROBAR EL MAIN
void mostrar_cache(const t_lru_cache *cache)
{
    tNodo *act = cache->lista;
    printf("--- ESTADO CACHE (Cap: %u/%u) [FRENTE -> FONDO] ---\n", cache->cap_act, cache->cap_max);
    if (!act)
    {
        printf("Caché vacía.\n");
    }
    while (act)
    {
        t_item *it = (t_item *)act->info;
        printf("[ID: %d - %s] -> ", it->id, it->descripcion);
        act = act->sig;
    }
    printf("NULL\n----------------------------------------------------\n\n");
}

void sacar_ultimo_lista(tLista* p)
{
    while((*p)->sig)
    {
        p = &(*p)->sig;
    }

    tNodo* aux = *p; ///ULTIMO
    *p = NULL;
    free(aux->info);
    free(aux);
}
