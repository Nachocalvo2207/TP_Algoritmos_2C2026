#include "Parte_3.h"

int comparar_posteos(const void *a, const void *b)
{
    const t_posteo *p1 = (const t_posteo *)a;
    const t_posteo *p2 = (const t_posteo *)b;
    return p1->id - p2->id;
}

// Genera los 50.000 posteos donde id == posicion relativa del registro
int generar_archivo_posteos(const char *ruta_posteos)
{
    FILE *pf = fopen(ruta_posteos, "wb");
    if (!pf) return ERROR;

    t_posteo post;
    for (int i = 0; i < CANT_POSTEOS; i++)
    {
        post.id = i;
        snprintf(post.texto, sizeof(post.texto),
                 "Tweener Post #%d: Este es un texto de prueba de menos de 140 caracteres para simular la red social.", i);
        fwrite(&post, sizeof(t_posteo), 1, pf);
    }

    fclose(pf);
    return TODO_OK;
}

// Lote Concentrado: Muchos usuarios ven los mismos posteos (ej. 150 posteos "virales")
int generar_lote_concentrado(const char *ruta_usuarios, unsigned int semilla)
{
    FILE *pf = fopen(ruta_usuarios, "wb");
    if (!pf) return ERROR;

    srand(semilla);
    t_usuario usu;

    for (int u = 0; u < CANT_USUARIOS; u++)
    {
        snprintf(usu.usuario, sizeof(usu.usuario), "user_%04d", u);
        for (int f = 0; f < TAM_FEED; f++)
        {
            // 90% de probabilidad de ver uno de los primeros 150 posteos virales
            if ((rand() % 100) < 90)
                usu.feed[f] = rand() % 150;
            else
                usu.feed[f] = rand() % CANT_POSTEOS;
        }
        fwrite(&usu, sizeof(t_usuario), 1, pf);
    }

    fclose(pf);
    return TODO_OK;
}

// Lote Disperso: Los usuarios ven posteos mayormente diferentes en todo el rango [0, 49999]
int generar_lote_disperso(const char *ruta_usuarios, unsigned int semilla)
{
    FILE *pf = fopen(ruta_usuarios, "wb");
    if (!pf) return ERROR;

    srand(semilla);
    t_usuario usu;

    for (int u = 0; u < CANT_USUARIOS; u++)
    {
        snprintf(usu.usuario, sizeof(usu.usuario), "user_%04d", u);
        for (int f = 0; f < TAM_FEED; f++)
        {
            // Aseguramos rango completo de 0 a 49999 combinando rand()
            unsigned int r = ((unsigned int)rand() << 15) ^ (unsigned int)rand();
            usu.feed[f] = r % CANT_POSTEOS;
        }
        fwrite(&usu, sizeof(t_usuario), 1, pf);
    }

    fclose(pf);
    return TODO_OK;
}

// Lote Ordenado: Los IDs de los feeds aparecen en orden creciente
int generar_lote_ordenado(const char *ruta_usuarios, unsigned int semilla)
{
    FILE *pf = fopen(ruta_usuarios, "wb");
    if (!pf) return ERROR;

    srand(semilla);
    t_usuario usu;
    int id_actual = semilla % 100; // Punto de partida según semilla

    for (int u = 0; u < CANT_USUARIOS; u++)
    {
        snprintf(usu.usuario, sizeof(usu.usuario), "user_%04d", u);
        for (int f = 0; f < TAM_FEED; f++)
        {
            // Avanzamos en orden creciente (con algunas repeticiones para evaluar hits y desbordes ordenados)
            if (f % 4 == 0 && f > 0)
                usu.feed[f] = id_actual; // Repite el último
            else
                usu.feed[f] = (++id_actual) % CANT_POSTEOS;
        }
        fwrite(&usu, sizeof(t_usuario), 1, pf);
    }

    fclose(pf);
    return TODO_OK;
}

int generar_todos_los_lotes(unsigned int semilla)
{
    printf("Generando archivo de %d posteos...\n", CANT_POSTEOS);
    if (generar_archivo_posteos(ARCH_POSTEOS) != TODO_OK) return ERROR;

    printf("Generando lotes de usuarios (%d usuarios c/u, semilla=%u)...\n", CANT_USUARIOS, semilla);
    if (generar_lote_concentrado(ARCH_USU_CONCENTRADO, semilla) != TODO_OK) return ERROR;
    if (generar_lote_disperso(ARCH_USU_DISPERSO, semilla) != TODO_OK) return ERROR;
    if (generar_lote_ordenado(ARCH_USU_ORDENADO, semilla) != TODO_OK) return ERROR;

    printf("¡Archivos generados correctamente!\n\n");
    return TODO_OK;
}

// Auxiliar interna: lectura directa en archivo binario de posteos por posición relativa (ID)
static int leer_posteo_de_archivo(FILE *pf_posteos, int id, t_posteo *post_out)
{
    if (fseek(pf_posteos, (long)id * sizeof(t_posteo), SEEK_SET) != 0)
        return ERROR;
    if (fread(post_out, sizeof(t_posteo), 1, pf_posteos) != 1)
        return ERROR;
    return TODO_OK;
}

// 1. SIMULACIÓN SIN CACHÉ
int simular_sin_cache(const char *ruta_posteos, const char *ruta_usuarios, t_resultado_sim *res)
{
    FILE *pf_post = fopen(ruta_posteos, "rb");
    FILE *pf_usu = fopen(ruta_usuarios, "rb");
    if (!pf_post || !pf_usu)
    {
        if (pf_post) fclose(pf_post);
        if (pf_usu) fclose(pf_usu);
        return ERROR;
    }

    memset(res, 0, sizeof(t_resultado_sim));
    t_usuario usu;
    t_posteo post;
    unsigned long cant_usuarios = 0;

    clock_t inicio = clock();

    while (fread(&usu, sizeof(t_usuario), 1, pf_usu) == 1)
    {
        cant_usuarios++;
        for (int i = 0; i < TAM_FEED; i++)
        {
            leer_posteo_de_archivo(pf_post, usu.feed[i], &post);
            res->lecturas_archivo++;
        }
    }

    clock_t fin = clock();

    unsigned long total_posteos = cant_usuarios * TAM_FEED;
    res->tiempo_total_ms = ((double)(fin - inicio) * 1000.0) / CLOCKS_PER_SEC;
    res->tiempo_promedio_usuario_ms = cant_usuarios ? (res->tiempo_total_ms / cant_usuarios) : 0;
    res->tiempo_promedio_posteo_ms = total_posteos ? (res->tiempo_total_ms / total_posteos) : 0;

    fclose(pf_post);
    fclose(pf_usu);
    return TODO_OK;
}

// 2. SIMULACIÓN CON CACHÉ PARTE 1 (Lista Simplemente Enlazada)
int simular_con_cache_P1(const char *ruta_posteos, const char *ruta_usuarios, unsigned cap_cache, t_resultado_sim *res)
{
    FILE *pf_post = fopen(ruta_posteos, "rb");
    FILE *pf_usu = fopen(ruta_usuarios, "rb");
    if (!pf_post || !pf_usu)
    {
        if (pf_post) fclose(pf_post);
        if (pf_usu) fclose(pf_usu);
        return ERROR;
    }

    memset(res, 0, sizeof(t_resultado_sim));
    t_lru_cache cache;
    crear_lrucache_P1(&cache, cap_cache, comparar_posteos);

    t_usuario usu;
    t_posteo post;
    unsigned long cant_usuarios = 0;

    clock_t inicio = clock();

    while (fread(&usu, sizeof(t_usuario), 1, pf_usu) == 1)
    {
        cant_usuarios++;
        for (int i = 0; i < TAM_FEED; i++)
        {
            post.id = usu.feed[i];
            if (obtener_lrucache_P1(&cache, &post, sizeof(t_posteo)) == TODO_OK)
            {
                res->aciertos++;
            }
            else
            {
                res->fallos++;
                leer_posteo_de_archivo(pf_post, usu.feed[i], &post);
                res->lecturas_archivo++;
                agregar_lrucache_P1(&cache, &post, sizeof(t_posteo));
            }
        }
    }

    clock_t fin = clock();

    unsigned long total_posteos = cant_usuarios * TAM_FEED;
    res->tiempo_total_ms = ((double)(fin - inicio) * 1000.0) / CLOCKS_PER_SEC;
    res->tiempo_promedio_usuario_ms = cant_usuarios ? (res->tiempo_total_ms / cant_usuarios) : 0;
    res->tiempo_promedio_posteo_ms = total_posteos ? (res->tiempo_total_ms / total_posteos) : 0;

    vaciar_lrucache_P1(&cache);
    fclose(pf_post);
    fclose(pf_usu);
    return TODO_OK;
}

// 3. SIMULACIÓN CON CACHÉ PARTE 2 (LDE + Árbol Binario de Búsqueda)
int simular_con_cache_P2(const char *ruta_posteos, const char *ruta_usuarios, unsigned cap_cache, t_resultado_sim *res)
{
    FILE *pf_post = fopen(ruta_posteos, "rb");
    FILE *pf_usu = fopen(ruta_usuarios, "rb");
    if (!pf_post || !pf_usu)
    {
        if (pf_post) fclose(pf_post);
        if (pf_usu) fclose(pf_usu);
        return ERROR;
    }

    memset(res, 0, sizeof(t_resultado_sim));
    t_lru_cache_P2 cache;
    crear_lrucache_P2(&cache, cap_cache, comparar_posteos);

    t_usuario usu;
    t_posteo post;
    unsigned long cant_usuarios = 0;

    clock_t inicio = clock();

    while (fread(&usu, sizeof(t_usuario), 1, pf_usu) == 1)
    {
        cant_usuarios++;
        for (int i = 0; i < TAM_FEED; i++)
        {
            post.id = usu.feed[i];
            if (obtener_lrucache_P2(&cache, &post, sizeof(t_posteo)) == TODO_OK)
            {
                res->aciertos++;
            }
            else
            {
                res->fallos++;
                leer_posteo_de_archivo(pf_post, usu.feed[i], &post);
                res->lecturas_archivo++;
                agregar_lrucache_P2(&cache, &post, sizeof(t_posteo));
            }
        }
    }

    clock_t fin = clock();

    res->altura_arbol = altura_lrucache_P2(&cache);

    unsigned long total_posteos = cant_usuarios * TAM_FEED;
    res->tiempo_total_ms = ((double)(fin - inicio) * 1000.0) / CLOCKS_PER_SEC;
    res->tiempo_promedio_usuario_ms = cant_usuarios ? (res->tiempo_total_ms / cant_usuarios) : 0;
    res->tiempo_promedio_posteo_ms = total_posteos ? (res->tiempo_total_ms / total_posteos) : 0;

    vaciar_lrucache_P2(&cache);
    fclose(pf_post);
    fclose(pf_usu);
    return TODO_OK;
}

void imprimir_encabezado_tabla(const char *nombre_lote)
{
    printf("============================================================================================================\n");
    printf(" LOTE: %s\n", nombre_lote);
    printf("============================================================================================================\n");
    printf("%-12s | %-8s | %-12s | %-12s | %-12s | %-9s | %-9s | %-9s | %-6s\n",
           "Metodo", "CapCache", "T.Total(ms)", "T.Usu(ms)", "T.Post(ms)", "Aciertos", "Fallos", "Lecturas", "Altura");
    printf("------------------------------------------------------------------------------------------------------------\n");
}

void imprimir_fila_resultado(const char *metodo, unsigned cap_cache, const t_resultado_sim *res, int mostrar_altura)
{
    char str_cap[16], str_alt[16];
    if (cap_cache == 0) strcpy(str_cap, "N/A");
    else snprintf(str_cap, sizeof(str_cap), "%u", cap_cache);

    if (!mostrar_altura) strcpy(str_alt, "-");
    else snprintf(str_alt, sizeof(str_alt), "%u", res->altura_arbol);

    printf("%-12s | %-8s | %12.3f | %12.5f | %12.6f | %9lu | %9lu | %9lu | %-6s\n",
           metodo, str_cap,
           res->tiempo_total_ms,
           res->tiempo_promedio_usuario_ms,
           res->tiempo_promedio_posteo_ms,
           res->aciertos, res->fallos, res->lecturas_archivo, str_alt);
}
