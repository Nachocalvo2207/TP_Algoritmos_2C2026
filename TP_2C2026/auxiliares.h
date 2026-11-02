#ifndef AUXILIARES_H_INCLUDED
#define AUXILIARES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lru_cache.h"

int  cmpInt(const void *a, const void *b);
int  cmpStr(const void *a, const void *b);

void mostrarIntStr(const void *clave, const void *valor, void *params);
void mostrarStrInt(const void *clave, const void *valor, void *params);

void mostrarCache(const t_lru_cache *c,
                  void (*accion)(const void *, const void *, void *));

int  consistente(const t_lru_cache *c);

int  pruebaAleatoria(unsigned cap, int rangoClaves, int ops, unsigned semilla);

#endif
