#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json_utils.h"
#include "cJSON.h"

// Función para cargar datos desde un archivo JSON
int cargarDatosDesdeArchivo(const char *rutaArchivo, ListaVentas *listaVentas) {
    FILE *archivo = fopen(rutaArchivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return 0;
    }

    // Leer todo el archivo en un buffer
    fseek(archivo, 0, SEEK_END);
    long tamanoArchivo = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *buffer = (char *)malloc(tamanoArchivo + 1);
    if (!buffer) {
        perror("Error al asignar memoria");
        fclose(archivo);
        return 0;
    }

    size_t leidos = fread(buffer, 1, tamanoArchivo, archivo);
    buffer[leidos] = '\0';
    fclose(archivo);

    // Parsear el buffer JSON
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        printf("Error al parsear el JSON: %s\n", cJSON_GetErrorPtr());
        free(buffer);
        return 0;
    }

    // Verificar que el JSON es un array
    if (!cJSON_IsArray(json)) {
        printf("El JSON no es un array.\n");
        cJSON_Delete(json);
        free(buffer);
        return 0;
    }

    // Iterar sobre el array de ventas en el JSON
    cJSON *ventaJson = NULL;
    cJSON_ArrayForEach(ventaJson, json) {
        if (!cJSON_IsObject(ventaJson)) {
            printf("Elemento no es un objeto.\n");
            continue;
        }

        Venta nuevaVenta;

        cJSON *item = cJSON_GetObjectItem(ventaJson, "venta_id");
        if (cJSON_IsNumber(item)) {
            nuevaVenta.venta_id = item->valueint;
        } else {
            printf("venta_id inválido o faltante.\n");
            continue;
        }

        item = cJSON_GetObjectItem(ventaJson, "fecha");
        if (cJSON_IsString(item)) {
            strncpy(nuevaVenta.fecha, item->valuestring, sizeof(nuevaVenta.fecha) - 1);
            nuevaVenta.fecha[sizeof(nuevaVenta.fecha) - 1] = '\0';
        } else {
            printf("fecha inválida o faltante en venta_id %d.\n", nuevaVenta.venta_id);
            continue;
        }

        item = cJSON_GetObjectItem(ventaJson, "producto_id");
        if (cJSON_IsNumber(item)) {
            nuevaVenta.producto_id = item->valueint;
        } else {
            printf("producto_id inválido o faltante en venta_id %d.\n", nuevaVenta.venta_id);
            continue;
        }

        item = cJSON_GetObjectItem(ventaJson, "producto_nombre");
        if (cJSON_IsString(item)) {
            strncpy(nuevaVenta.producto_nombre, item->valuestring, sizeof(nuevaVenta.producto_nombre) - 1);
            nuevaVenta.producto_nombre[sizeof(nuevaVenta.producto_nombre) - 1] = '\0';
        } else {
            printf("producto_nombre inválido o faltante en venta_id %d.\n", nuevaVenta.venta_id);
            continue;
        }

        item = cJSON_GetObjectItem(ventaJson, "categoria");
        if (cJSON_IsString(item)) {
            strncpy(nuevaVenta.categoria, item->valuestring, sizeof(nuevaVenta.categoria) - 1);
            nuevaVenta.categoria[sizeof(nuevaVenta.categoria) - 1] = '\0';
        } else {
            printf("categoria inválida o faltante en venta_id %d.\n", nuevaVenta.venta_id);
            continue;
        }

        item = cJSON_GetObjectItem(ventaJson, "cantidad");
        if (cJSON_IsNumber(item)) {
            nuevaVenta.cantidad = item->valueint;
        } else {
            printf("cantidad inválida o faltante en venta_id %d.\n", nuevaVenta.venta_id);
            continue;
        }

        item = cJSON_GetObjectItem(ventaJson, "precio_unitario");
        if (cJSON_IsNumber(item)) {
            nuevaVenta.precio_unitario = item->valuedouble;
        } else {
            printf("precio_unitario inválido o faltante en venta_id %d.\n", nuevaVenta.venta_id);
            continue;
        }

        item = cJSON_GetObjectItem(ventaJson, "total");
        if (cJSON_IsNumber(item)) {
            nuevaVenta.total = item->valuedouble;
        } else {
            printf("total inválido o faltante en venta_id %d.\n", nuevaVenta.venta_id);
            continue;
        }

        agregarVenta(listaVentas, nuevaVenta);
        printf("Venta agregada: ID %d, Fecha %s, Total %.2f\n", nuevaVenta.venta_id, nuevaVenta.fecha, nuevaVenta.total);

    }

    // Liberar recursos
    cJSON_Delete(json);
    free(buffer);
    return 1;
}