#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data_structures.h"
#include "cJSON.h"

#define CAPACIDAD_INICIAL 10

// Inicializa la lista dinámica de ventas
void inicializarListaVentas(ListaVentas *lista) {
    lista->ventas = (Venta *)malloc(CAPACIDAD_INICIAL * sizeof(Venta));
    lista->tamano = 0;
    lista->capacidad = CAPACIDAD_INICIAL;
}

void agregarVenta(ListaVentas *lista, Venta venta) {
    // Verificar si la lista necesita más capacidad
    if (lista->tamano >= lista->capacidad) {
        size_t nuevaCapacidad = lista->capacidad == 0 ? 10 : lista->capacidad * 2;
        Venta *nuevasVentas = (Venta *)realloc(lista->ventas, nuevaCapacidad * sizeof(Venta));
        if (nuevasVentas == NULL) {
            printf("Error: No se pudo asignar memoria adicional para las ventas.\n");
            return;
        }
        lista->ventas = nuevasVentas;
        lista->capacidad = nuevaCapacidad;
        printf("Capacidad de la lista aumentada a %zu.\n", nuevaCapacidad);
    }

    // Agregar la nueva venta a la lista
    lista->ventas[lista->tamano] = venta;
    printf("Venta añadida: %d - %s - %s\n", venta.venta_id, venta.producto_nombre, venta.categoria);
    lista->tamano++;
    printf("Total de ventas ahora: %zu\n", lista->tamano);
}
// Libera la memoria utilizada por la lista de ventas
void liberarListaVentas(ListaVentas *lista) {
    free(lista->ventas);
    lista->ventas = NULL;
    lista->tamano = 0;
    lista->capacidad = 0;
}

void imprimirListaVentas(const ListaVentas *lista) {
    printf("\n--- Lista de Ventas ---\n");
    for (size_t i = 0; i < lista->tamano; i++) {
        Venta v = lista->ventas[i];
        printf("Venta ID: %d\n", v.venta_id);
        printf("Fecha: %s\n", v.fecha);
        printf("Producto ID: %d\n", v.producto_id);
        printf("Producto Nombre: %s\n", v.producto_nombre);
        printf("Categoria: %s\n", v.categoria);
        printf("Cantidad: %d\n", v.cantidad);
        printf("Precio Unitario: %.2f\n", v.precio_unitario);
        printf("Total: %.2f\n", v.total);
        printf("--------------------------\n");
    }
}

void guardarDatosJSON(const ListaVentas *lista, const char *nombreArchivo) {
    cJSON *root = cJSON_CreateArray();

    for (size_t i = 0; i < lista->tamano; i++) {
        cJSON *ventaJSON = cJSON_CreateObject();
        cJSON_AddNumberToObject(ventaJSON, "venta_id", lista->ventas[i].venta_id);
        cJSON_AddStringToObject(ventaJSON, "fecha", lista->ventas[i].fecha);
        cJSON_AddNumberToObject(ventaJSON, "producto_id", lista->ventas[i].producto_id);
        cJSON_AddStringToObject(ventaJSON, "producto_nombre", lista->ventas[i].producto_nombre);
        cJSON_AddStringToObject(ventaJSON, "categoria", lista->ventas[i].categoria);
        cJSON_AddNumberToObject(ventaJSON, "cantidad", lista->ventas[i].cantidad);
        cJSON_AddNumberToObject(ventaJSON, "precio_unitario", lista->ventas[i].precio_unitario);
        cJSON_AddNumberToObject(ventaJSON, "total", lista->ventas[i].total);
        cJSON_AddItemToArray(root, ventaJSON);
    }

    char *stringJSON = cJSON_Print(root);
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo != NULL) {
        fprintf(archivo, "%s", stringJSON);
        fclose(archivo);
    }

    cJSON_Delete(root);
    free(stringJSON);
}

void cargarDatosJSON(ListaVentas *lista, const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", nombreArchivo);
        return;
    }

    // Leer todo el contenido del archivo
    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);
    char *contenidoArchivo = (char *)malloc(tamanoArchivo + 1);
    fread(contenidoArchivo, 1, tamanoArchivo, archivo);
    contenidoArchivo[tamanoArchivo] = '\0';
    fclose(archivo);

    // Parsear el JSON
    cJSON *json = cJSON_Parse(contenidoArchivo);
    free(contenidoArchivo);
    if (json == NULL) {
        printf("Error: No se pudo parsear el JSON.\n");
        return;
    }

    cJSON *ventaJSON = NULL;
    cJSON_ArrayForEach(ventaJSON, json) {
        Venta venta;
        
        // Parsear cada campo de la venta
        cJSON *venta_id = cJSON_GetObjectItemCaseSensitive(ventaJSON, "venta_id");
        cJSON *fecha = cJSON_GetObjectItemCaseSensitive(ventaJSON, "fecha");
        cJSON *producto_id = cJSON_GetObjectItemCaseSensitive(ventaJSON, "producto_id");
        cJSON *producto_nombre = cJSON_GetObjectItemCaseSensitive(ventaJSON, "producto_nombre");
        cJSON *categoria = cJSON_GetObjectItemCaseSensitive(ventaJSON, "categoria");
        cJSON *cantidad = cJSON_GetObjectItemCaseSensitive(ventaJSON, "cantidad");
        cJSON *precio_unitario = cJSON_GetObjectItemCaseSensitive(ventaJSON, "precio_unitario");
        cJSON *total = cJSON_GetObjectItemCaseSensitive(ventaJSON, "total");

        if (cJSON_IsNumber(venta_id) && cJSON_IsString(fecha) && cJSON_IsNumber(producto_id) &&
            cJSON_IsString(producto_nombre) && cJSON_IsString(categoria) && 
            cJSON_IsNumber(cantidad) && cJSON_IsNumber(precio_unitario) && cJSON_IsNumber(total)) {

            venta.venta_id = venta_id->valueint;
            strcpy(venta.fecha, fecha->valuestring);
            venta.producto_id = producto_id->valueint;
            strcpy(venta.producto_nombre, producto_nombre->valuestring);
            strcpy(venta.categoria, categoria->valuestring);
            venta.cantidad = cantidad->valueint;
            venta.precio_unitario = precio_unitario->valuedouble;
            venta.total = total->valuedouble;

            // Intentar agregar la venta
            printf("Intentando agregar la venta ID %d\n", venta.venta_id);
            agregarVenta(lista, venta);
        } else {
            printf("Error: Faltan campos en la venta ID %d o están mal formados.\n", venta_id->valueint);
        }
    }

    cJSON_Delete(json);
    printf("Datos cargados. Total de ventas: %zu\n", lista->tamano);
}
