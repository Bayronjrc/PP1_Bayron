#include <stdlib.h>
#include <string.h>
#include "data_structures.h"

#define CAPACIDAD_INICIAL 10

// Inicializa la lista dinámica de ventas
void inicializarListaVentas(ListaVentas *lista) {
    lista->ventas = (Venta *)malloc(CAPACIDAD_INICIAL * sizeof(Venta));
    lista->tamano = 0;
    lista->capacidad = CAPACIDAD_INICIAL;
}

// Agrega una nueva venta a la lista, redimensionando si es necesario
void agregarVenta(ListaVentas *lista, Venta nuevaVenta) {
    if (lista->tamano == lista->capacidad) {
        lista->capacidad *= 2;
        lista->ventas = (Venta *)realloc(lista->ventas, lista->capacidad * sizeof(Venta));
    }
    lista->ventas[lista->tamano++] = nuevaVenta;
}

// Libera la memoria utilizada por la lista de ventas
void liberarListaVentas(ListaVentas *lista) {
    free(lista->ventas);
    lista->ventas = NULL;
    lista->tamano = 0;
    lista->capacidad = 0;
}
