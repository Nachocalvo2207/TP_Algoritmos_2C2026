#include "Parte_2.h"


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
