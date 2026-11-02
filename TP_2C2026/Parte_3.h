#ifndef PARTE_3_H_INCLUDED
#define PARTE_3_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Parte_1.h"
#include "Parte_2_pruebas.h"

#define CANT_POSTEOS 50000
#define CANT_USUARIOS 1000   // >= 200 registros como pide el enunciado
#define TAM_FEED 20

#define ARCH_POSTEOS "posteos.dat"
#define ARCH_USU_CONCENTRADO "usuarios_concentrado.dat"
#define ARCH_USU_DISPERSO "usuarios_disperso.dat"
#define ARCH_USU_ORDENADO "usuarios_ordenado.dat"

typedef struct {
    int id;
    char texto[141];
} t_posteo;

typedef struct {
    char usuario[15];
    int feed[TAM_FEED];
} t_usuario;

typedef struct {
    double tiempo_total_ms;
    double tiempo_promedio_usuario_ms;
    double tiempo_promedio_posteo_ms;
    unsigned long aciertos;
    unsigned long fallos;
    unsigned long lecturas_archivo;
    unsigned altura_arbol;
} t_resultado_sim;

// Funciones de generación de lotes
int generar_archivo_posteos(const char *ruta_posteos);
int generar_lote_concentrado(const char *ruta_usuarios, unsigned int semilla);
int generar_lote_disperso(const char *ruta_usuarios, unsigned int semilla);
int generar_lote_ordenado(const char *ruta_usuarios, unsigned int semilla);
int generar_todos_los_lotes(unsigned int semilla);

// Comparador de posteos por ID
int comparar_posteos(const void *a, const void *b);

// Funciones de simulación
int simular_sin_cache(const char *ruta_posteos, const char *ruta_usuarios, t_resultado_sim *res);
int simular_con_cache_P1(const char *ruta_posteos, const char *ruta_usuarios, unsigned cap_cache, t_resultado_sim *res);
int simular_con_cache_P2(const char *ruta_posteos, const char *ruta_usuarios, unsigned cap_cache, t_resultado_sim *res);

void imprimir_encabezado_tabla(const char *nombre_lote);
void imprimir_fila_resultado(const char *metodo, unsigned cap_cache, const t_resultado_sim *res, int mostrar_altura);

#endif // PARTE_3_H_INCLUDED
