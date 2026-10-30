#include "Parte_1.h"

int main()
{
    t_lru_cache cache;

    // Creamos una caché pequeña de capacidad máxima 3 para probar rápido el desborde
    crear_lrucache_P1(&cache, 3, comparar_items);
    printf("Caché creada con capacidad maxima = 3.\n\n");

    t_item i1 = {1, "Posteo Uno"};
    t_item i2 = {2, "Posteo Dos"};
    t_item i3 = {3, "Posteo Tres"};
    t_item i4 = {4, "Posteo Cuatro"}; // Este va a provocar desborde

    // 1. Probamos agregando elementos
    printf(">>> Agregando items 1, 2 y 3...\n");
    agregar_lrucache_P1(&cache, &i1, sizeof(t_item));
    agregar_lrucache_P1(&cache, &i2, sizeof(t_item));
    agregar_lrucache_P1(&cache, &i3, sizeof(t_item));
    mostrar_cache(&cache); // Esperado: [3] -> [2] -> [1] (el último agregado queda al frente)

    // 2. Probamos obtener un elemento existente (Hit) y ver cómo pasa al frente (LRU)
    printf(">>> Obteniendo el item con ID 1 (Debe pasar al frente)...\n");
    t_item busqueda = {1, ""}; // Solo necesitamos el ID para comparar
    if (obtener_lrucache_P1(&cache, &busqueda, sizeof(t_item)) == ENCONTRADO)
    {
        printf("¡Encontrado! Descripción recuperada: %s\n\n", busqueda.descripcion);
    }
    mostrar_cache(&cache); // Esperado: [1] -> [3] -> [2]

    // 3. Probamos el desborde al agregar un 4to elemento (Debe descartar al menos recientemente usado: ID 2)
    printf(">>> Agregando item 4 (Debe desbordar y eliminar al ID 2)...\n");
    agregar_lrucache_P1(&cache, &i4, sizeof(t_item));
    mostrar_cache(&cache); // Esperado: [4] -> [1] -> [3]

    // 4. Probamos reemplazar un elemento existente (Misma clave, se actualiza y pasa al frente)
    printf(">>> Actualizando el item con ID 3 ('Posteo Tres Modificado')...\n");
    t_item i3_mod = {3, "Posteo Tres Modificado"};
    agregar_lrucache_P1(&cache, &i3_mod, sizeof(t_item));
    mostrar_cache(&cache); // Esperado: [3] -> [4] -> [1]

    // 5. Probamos borrar un elemento específico por clave
    printf(">>> Borrando el item con ID 4...\n");
    t_item a_borrar = {4, ""};
    borrar_lrucache_P1(&cache, &a_borrar);
    mostrar_cache(&cache); // Esperado: [3] -> [1]

    // 6. Vaciamos y liberamos toda la memoria
    vaciar_lrucache_P1(&cache);
    printf(">>> Caché vaciada.\n");
    mostrar_cache(&cache);

    return 0;
}
