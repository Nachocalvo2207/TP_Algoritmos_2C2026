#include "Parte_3.h"

void ejecutar_bateria_lote(const char *nombre_lote, const char *ruta_usu, int es_ordenado)
{
    unsigned capacidades[] = {50, 150, 500, 1500, 3000};
    int cant_caps = sizeof(capacidades) / sizeof(capacidades[0]);
    t_resultado_sim res;

    imprimir_encabezado_tabla(nombre_lote);

    // 1. Ejecución base Sin Caché
    simular_sin_cache(ARCH_POSTEOS, ruta_usu, &res);
    imprimir_fila_resultado("Sin Cache", 0, &res, 0);
    printf("------------------------------------------------------------------------------------------------------------\n");

    // 2. Ejecución comparando Parte 1 y Parte 2 para cada capacidad de caché
    for (int i = 0; i < cant_caps; i++)
    {
        unsigned cap = capacidades[i];

        simular_con_cache_P1(ARCH_POSTEOS, ruta_usu, cap, &res);
        imprimir_fila_resultado("Cache P1", cap, &res, 0);

        simular_con_cache_P2(ARCH_POSTEOS, ruta_usu, cap, &res);
        imprimir_fila_resultado("Cache P2", cap, &res, es_ordenado ? 1 : 1); // Mostramos altura siempre o en ordenado
        printf("------------------------------------------------------------------------------------------------------------\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    unsigned int semilla = 12345; // Semilla por defecto reproducible

    if (argc > 1)
        semilla = (unsigned int)atoi(argv[1]);
    else
    {
        printf("Ingrese una semilla numerica para generar los lotes (ej. 12345): ");
        if (scanf("%u", &semilla) != 1)
            semilla = 12345;
    }

    if (generar_todos_los_lotes(semilla) != TODO_OK)
    {
        printf("Error al generar los archivos de prueba.\n");
        return 1;
    }

    ejecutar_bateria_lote("LOTE CONCENTRADO (Muchos usuarios ven los mismos posteos)", ARCH_USU_CONCENTRADO, 0);
    ejecutar_bateria_lote("LOTE DISPERSO (Usuarios ven posteos mayormente diferentes)", ARCH_USU_DISPERSO, 0);
    ejecutar_bateria_lote("LOTE ORDENADO (IDs en orden creciente - Analisis Altura ABB)", ARCH_USU_ORDENADO, 1);

    return 0;
}
