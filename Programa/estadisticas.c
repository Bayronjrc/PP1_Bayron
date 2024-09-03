#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "estadisticas.h"
#include "hpdf.h"
#include "data_structures.h"

typedef struct {
    char categoria[50];
    double montoTotal;
} CategoriaVenta;

// Función para comparar categorías por monto total (usada en qsort)
int compararCategorias(const void *a, const void *b) {
    CategoriaVenta *catA = (CategoriaVenta *)a;
    CategoriaVenta *catB = (CategoriaVenta *)b;
    return (catB->montoTotal > catA->montoTotal) - (catB->montoTotal < catA->montoTotal);
}

// Función para calcular y mostrar el Top 5 de categorías con mayores ventas
void mostrarTop5Categorias(const ListaVentas *lista) {
    CategoriaVenta categorias[100]; // Asumimos un máximo de 100 categorías diferentes
    int numCategorias = 0;

    // Iterar sobre las ventas y acumular montos por categoría
    for (size_t i = 0; i < lista->tamano; i++) {
        const char *categoriaActual = lista->ventas[i].categoria;
        double montoActual = lista->ventas[i].total;
        
        // Buscar si la categoría ya existe
        int encontrada = 0;
        for (int j = 0; j < numCategorias; j++) {
            if (strcmp(categorias[j].categoria, categoriaActual) == 0) {
                categorias[j].montoTotal += montoActual;
                encontrada = 1;
                break;
            }
        }
        
        // Si la categoría no existe, se agrega una nueva entrada
        if (!encontrada) {
            strcpy(categorias[numCategorias].categoria, categoriaActual);
            categorias[numCategorias].montoTotal = montoActual;
            numCategorias++;
        }
    }

    // Ordenar las categorías por monto total en orden descendente
    qsort(categorias, numCategorias, sizeof(CategoriaVenta), compararCategorias);

    // Mostrar el Top 5 de categorías con mayores ventas
    printf("Top 5 de categorías con mayores ventas:\n");
    for (int i = 0; i < numCategorias && i < 5; i++) {
        printf("%d. %s: %.2f\n", i + 1, categorias[i].categoria, categorias[i].montoTotal);
    }
}

void exportarTop5CategoriasPDF(const ListaVentas *lista) {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        printf("Error al crear el PDF.\n");
        return;
    }

    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);

    HPDF_Page_BeginText(page);
    HPDF_Page_SetFontAndSize(page, HPDF_GetFont(pdf, "Helvetica", NULL), 12);
    HPDF_Page_MoveTextPos(page, 50, 750);

    HPDF_Page_ShowText(page, "Top 5 de categorias con mayores ventas:");

    CategoriaVenta categorias[100];
    int numCategorias = 0;

    for (size_t i = 0; i < lista->tamano; i++) {
        const char *categoriaActual = lista->ventas[i].categoria;
        double montoActual = lista->ventas[i].total;

        int encontrada = 0;
        for (int j = 0; j < numCategorias; j++) {
            if (strcmp(categorias[j].categoria, categoriaActual) == 0) {
                categorias[j].montoTotal += montoActual;
                encontrada = 1;
                break;
            }
        }

        if (!encontrada) {
            strcpy(categorias[numCategorias].categoria, categoriaActual);
            categorias[numCategorias].montoTotal = montoActual;
            numCategorias++;
        }
    }

    qsort(categorias, numCategorias, sizeof(CategoriaVenta), compararCategorias);

    for (int i = 0; i < numCategorias && i < 5; i++) {
        char linea[100];
        sprintf(linea, "%d. %s: %.2f", i + 1, categorias[i].categoria, categorias[i].montoTotal);
        HPDF_Page_MoveTextPos(page, 0, -20);
        HPDF_Page_ShowText(page, linea);
    }

    HPDF_Page_EndText(page);
    HPDF_SaveToFile(pdf, "estadisticas_top5.pdf");
    HPDF_Free(pdf);

    printf("Top 5 exportado a PDF.\n");
}


// Función para mostrar estadísticas
void mostrarEstadisticas(const ListaVentas *lista) {
    printf("Mostrando estadisticas...\n");

    // Llama a la función para mostrar el Top 5 de categorías con mayores ventas
    mostrarTop5Categorias(lista);
    exportarTop5CategoriasPDF(lista);

    printf("Estadisticas mostradas.\n");
}
