#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "data_processing.h"

// Funciones de utilidad para calcular moda, media y mediana
double calcularMedia(double *valores, size_t tamano);
double calcularMediana(double *valores, size_t tamano);
double calcularModa(double *valores, size_t tamano);

// Completa los datos faltantes en cantidad y precio_unitario
void completarDatosFaltantes(ListaVentas *listaVentas) {
    size_t i;
    double *cantidades = (double *)malloc(listaVentas->tamano * sizeof(double));
    double *precios = (double *)malloc(listaVentas->tamano * sizeof(double));

    for (i = 0; i < listaVentas->tamano; i++) {
        cantidades[i] = (double)listaVentas->ventas[i].cantidad;
        precios[i] = listaVentas->ventas[i].precio_unitario;
    }

    double mediaCantidad = calcularMedia(cantidades, listaVentas->tamano);
    double medianaPrecio = calcularMediana(precios, listaVentas->tamano);

    for (i = 0; i < listaVentas->tamano; i++) {
        if (listaVentas->ventas[i].cantidad == 0) {
            listaVentas->ventas[i].cantidad = (int)round(mediaCantidad);
            printf("Completado cantidad en venta_id %d con media: %d\n", listaVentas->ventas[i].venta_id, listaVentas->ventas[i].cantidad);
        }
        if (listaVentas->ventas[i].precio_unitario == 0.0) {
            listaVentas->ventas[i].precio_unitario = medianaPrecio;
            printf("Completado precio_unitario en venta_id %d con mediana: %.2f\n", listaVentas->ventas[i].venta_id, listaVentas->ventas[i].precio_unitario);
        }
    }

    free(cantidades);
    free(precios);
}

// Elimina duplicados basándose en venta_id
void eliminarDuplicados(ListaVentas *listaVentas) {
    for (size_t i = 0; i < listaVentas->tamano - 1; i++) {
        for (size_t j = i + 1; j < listaVentas->tamano; j++) {
            if (listaVentas->ventas[i].venta_id == listaVentas->ventas[j].venta_id) {
                // Eliminar el elemento duplicado
                for (size_t k = j; k < listaVentas->tamano - 1; k++) {
                    listaVentas->ventas[k] = listaVentas->ventas[k + 1];
                }
                listaVentas->tamano--;
                j--; // Volver a comprobar la posición actual
                printf("Eliminado duplicado con venta_id %d\n", listaVentas->ventas[i].venta_id);
            }
        }
    }
}

// Implementación de las funciones de cálculo

double calcularMedia(double *valores, size_t tamano) {
    double suma = 0.0;
    for (size_t i = 0; i < tamano; i++) {
        suma += valores[i];
    }
    return suma / tamano;
}

int compararDoubles(const void *a, const void *b) {
    double diff = (*(double *)a - *(double *)b);
    return (diff > 0) - (diff < 0);
}

double calcularMediana(double *valores, size_t tamano) {
    qsort(valores, tamano, sizeof(double), compararDoubles);
    if (tamano % 2 == 0) {
        return (valores[tamano / 2 - 1] + valores[tamano / 2]) / 2.0;
    } else {
        return valores[tamano / 2];
    }
}

// Para simplicidad, podemos retornar la media como la moda en este caso
double calcularModa(double *valores, size_t tamano) {
    return calcularMedia(valores, tamano);
}
