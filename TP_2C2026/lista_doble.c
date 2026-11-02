#include "lista_doble.h"

void crearLista(tLista *p)
{
    *p = NULL;
}


int listaVacia(const tLista *p)
{
    return *p == NULL;
}


int vaciarLista(tLista *p)
{
    int    cant = 0;
    tNodo *act  = *p;

    if(act)
    {
        while(act->ant)
            act = act->ant;
        while(act)
        {
            tNodo *aux = act->sig;

            free(act->info);
            free(act);
            act = aux;
            cant++;
        }
        *p = NULL;
    }
    return cant;
}


tNodo *insertarPrimeroLista(tLista *p, const void *d, unsigned tam)
{
    tNodo *act = *p,
          *nue;

    if(act)
        while(act->ant)
            act = act->ant;

    if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
       (nue->info = malloc(tam)) == NULL)
    {
        free(nue);
        return NULL;
    }
    memcpy(nue->info, d, tam);
    nue->tamInfo = tam;
    nue->ant = NULL;
    nue->sig = act;
    if(act)
        act->ant = nue;
    *p = nue;
    return nue;
}


void ponerPrimeroLista(tLista *p, tNodo *nodo)
{
    tNodo *pri = *p;

    if(!nodo || !nodo->ant)
        return;

    while(pri->ant)
        pri = pri->ant;

    nodo->ant->sig = nodo->sig;
    if(nodo->sig)
        nodo->sig->ant = nodo->ant;

    nodo->ant = NULL;
    nodo->sig = pri;
    pri->ant  = nodo;
    *p = nodo;
}


int eliminarNodoLista(tLista *p, tNodo *nodo, void *d, unsigned tam)
{
    if(!nodo)
        return 0;

    if(nodo->ant)
        nodo->ant->sig = nodo->sig;
    if(nodo->sig)
        nodo->sig->ant = nodo->ant;
    if(*p == nodo)
        *p = nodo->sig ? nodo->sig : nodo->ant;

    if(d)
        memcpy(d, nodo->info, MINIMO(tam, nodo->tamInfo));
    free(nodo->info);
    free(nodo);
    return 1;
}


int modificarNodoLista(tNodo *nodo, const void *d, unsigned tam)
{
    if(tam != nodo->tamInfo)
    {
        void *aux = realloc(nodo->info, tam);
        if(!aux)
            return SIN_MEM;
        nodo->info    = aux;
        nodo->tamInfo = tam;
    }
    memcpy(nodo->info, d, tam);
    return TODO_BIEN;
}


void *infoNodoLista(const tNodo *nodo)
{
    return nodo->info;
}


tNodo *anteriorNodoLista(const tNodo *nodo)
{
    return nodo->ant;
}


int recorrerLista(const tLista *p, void *params,
                  void (*accion)(void *info, unsigned tam, void *params))
{
    const tNodo *act = *p;
    int          cant = 0;

    if(act)
        while(act->ant)
            act = act->ant;
    while(act)
    {
        accion(act->info, act->tamInfo, params);
        act = act->sig;
        cant++;
    }
    return cant;
}
