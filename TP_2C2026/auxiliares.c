#include "auxiliares.h"

#define MAX_MODELO 1000

int cmpInt(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}


int cmpStr(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}


void mostrarIntStr(const void *clave, const void *valor, void *params)
{
    (void)params;
    printf("[%d:%s] ", *(const int *)clave, (const char *)valor);
}


void mostrarStrInt(const void *clave, const void *valor, void *params)
{
    (void)params;
    printf("[%s:%d] ", (const char *)clave, *(const int *)valor);
}


void mostrarCache(const t_lru_cache *c,
                  void (*accion)(const void *, const void *, void *))
{
    printf("  MRU -> ");
    recorrer_lrucache(c, NULL, accion);
    printf("<- LRU   (cant=%u, altura indice=%u)\n",
           cantidad_lrucache(c), altura_indice_lrucache(c));
}


static void contar(const void *clave, const void *valor, void *params)
{
    (void)clave;
    (void)valor;
    (*(unsigned *)params)++;
}

int consistente(const t_lru_cache *c)
{
    unsigned n = 0;

    recorrer_lrucache(c, &n, contar);
    return n == cantidad_lrucache(c)
        && n <= c->capacidad
        && cantNodosArbolBin(&c->indice) == n;
}


typedef struct { int clave, valor; } tPar;
typedef struct { tPar v[MAX_MODELO]; int n, cap; } tModelo;
typedef struct { const tModelo *m; int i, ok; } tComparacion;

static int posModelo(const tModelo *m, int k)
{
    int i;
    for(i = 0; i < m->n; i++)
        if(m->v[i].clave == k)
            return i;
    return -1;
}

static void alFrenteModelo(tModelo *m, int i)
{
    tPar p = m->v[i];
    memmove(&m->v[1], &m->v[0], i * sizeof(tPar));
    m->v[0] = p;
}

static void agregarModelo(tModelo *m, int k, int val)
{
    int i = posModelo(m, k);
    if(i >= 0)
    {
        m->v[i].valor = val;
        alFrenteModelo(m, i);
        return;
    }
    if(m->n == m->cap)
        m->n--;
    memmove(&m->v[1], &m->v[0], m->n * sizeof(tPar));
    m->v[0].clave = k;
    m->v[0].valor = val;
    m->n++;
}

static void borrarModelo(tModelo *m, int i)
{
    memmove(&m->v[i], &m->v[i + 1], (m->n - i - 1) * sizeof(tPar));
    m->n--;
}

static void compararOrden(const void *k, const void *v, void *params)
{
    tComparacion *c = (tComparacion *)params;
    if(c->i >= c->m->n || c->m->v[c->i].clave != *(const int *)k
       || c->m->v[c->i].valor != *(const int *)v)
        c->ok = 0;
    c->i++;
}

static int igualAlModelo(const t_lru_cache *c, const tModelo *m)
{
    tComparacion comp;

    comp.m = m;
    comp.i = 0;
    comp.ok = 1;
    recorrer_lrucache(c, &comp, compararOrden);
    return comp.ok && comp.i == m->n;
}


int pruebaAleatoria(unsigned cap, int rangoClaves, int ops, unsigned semilla)
{
    t_lru_cache c;
    tModelo     m;
    int         i, k, v, r, esperado, fallos = 0;

    if(cap > MAX_MODELO)
        return 1;
    srand(semilla);
    crear_lrucache(&c, cap, cmpInt);
    m.n = 0;
    m.cap = cap;

    for(i = 0; i < ops && !fallos; i++)
    {
        k = rand() % rangoClaves;
        switch(rand() % 3)
        {
        case 0:
            v = rand();
            agregar_lrucache(&c, &k, sizeof k, &v, sizeof v);
            agregarModelo(&m, k, v);
            break;
        case 1:
            r = obtener_lrucache(&c, &k, &v, sizeof v);
            esperado = posModelo(&m, k);
            if((r == LRU_OK) != (esperado >= 0))
                fallos++;
            else if(esperado >= 0)
            {
                if(m.v[esperado].valor != v)
                    fallos++;
                alFrenteModelo(&m, esperado);
            }
            break;
        default:
            r = borrar_lrucache(&c, &k);
            esperado = posModelo(&m, k);
            if((r == LRU_OK) != (esperado >= 0))
                fallos++;
            else if(esperado >= 0)
                borrarModelo(&m, esperado);
        }
        if(!igualAlModelo(&c, &m) || !consistente(&c))
            fallos++;
    }
    vaciar_lrucache(&c);
    printf("  cap=%-4u claves=%-4d ops=%d -> %s\n", cap, rangoClaves, ops,
           fallos ? "FALLA" : "OK");
    return fallos;
}
