#include "auxiliares.h"

int main(void)
{
    t_lru_cache c;
    int         k, i, fallos = 0;
    char        txt[20];

    printf("=== Demo claves int / valores string, capacidad 3 ===\n");
    crear_lrucache(&c, 3, cmpInt);
    k = 1; agregar_lrucache(&c, &k, sizeof k, "uno",  4);
    k = 2; agregar_lrucache(&c, &k, sizeof k, "dos",  4);
    k = 3; agregar_lrucache(&c, &k, sizeof k, "tres", 5);
    mostrarCache(&c, mostrarIntStr);

    k = 1;
    if(obtener_lrucache(&c, &k, txt, sizeof txt) == LRU_OK)
        printf("obtener(1) = %s\n", txt);
    mostrarCache(&c, mostrarIntStr);

    k = 4; agregar_lrucache(&c, &k, sizeof k, "cuatro", 7);
    printf("agregar(4) -> desborda, sale el 2\n");
    mostrarCache(&c, mostrarIntStr);

    k = 2;
    printf("obtener(2) -> %s\n",
           obtener_lrucache(&c, &k, txt, sizeof txt) == LRU_OK ? "existe" : "no existe");

    k = 3; agregar_lrucache(&c, &k, sizeof k, "TRES (reemplazado)", 19);
    printf("agregar(3) con clave existente -> reemplaza y pasa primero\n");
    mostrarCache(&c, mostrarIntStr);

    k = 1; borrar_lrucache(&c, &k);
    printf("borrar(1)\n");
    mostrarCache(&c, mostrarIntStr);
    printf("consistente: %s\n", consistente(&c) ? "si" : "NO");
    vaciar_lrucache(&c);

    printf("\n=== Demo claves string / valores int, capacidad 2 ===\n");
    crear_lrucache(&c, 2, cmpStr);
    k = 10; agregar_lrucache(&c, "ana",  4, &k, sizeof k);
    k = 20; agregar_lrucache(&c, "beto", 5, &k, sizeof k);
    obtener_lrucache(&c, "ana", &k, sizeof k);
    k = 30; agregar_lrucache(&c, "caro", 5, &k, sizeof k);
    mostrarCache(&c, mostrarStrInt);
    vaciar_lrucache(&c);

    printf("\n=== Claves en orden creciente (como el lote ordenado) ===\n");
    crear_lrucache(&c, 100, cmpInt);
    for(i = 0; i < 1000; i++)
        agregar_lrucache(&c, &i, sizeof i, &i, sizeof i);
    printf("  cantidad=%u  altura del indice=%u (arbol degenerado en lista)\n",
           cantidad_lrucache(&c), altura_indice_lrucache(&c));
    printf("  consistente: %s\n", consistente(&c) ? "si" : "NO");
    vaciar_lrucache(&c);

    printf("\n=== Pruebas aleatorias contra un modelo de referencia ===\n");
    fallos += pruebaAleatoria(1,   5,    20000, 1);
    fallos += pruebaAleatoria(3,   10,   50000, 2);
    fallos += pruebaAleatoria(10,  30,   50000, 3);
    fallos += pruebaAleatoria(50,  200,  50000, 4);
    fallos += pruebaAleatoria(200, 300,  20000, 5);

    printf("\n%s\n", fallos ? "HUBO FALLAS" : "Todas las pruebas OK");
    return fallos != 0;
}
