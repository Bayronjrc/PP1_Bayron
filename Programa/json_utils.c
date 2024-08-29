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
    fread(buffer, 1, tamanoArchivo, archivo);
    buffer[tamanoArchivo] = '\0';
    fclose(archivo);

    // Parsear el buffer JSON
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        printf("Error al parsear el JSON: %s\n", cJSON_GetErrorPtr());
        free(buffer);
        return 0;
    }

    // Iterar sobre el array de ventas en el JSON
    cJSON *ventaJson = NULL;
    cJSON_ArrayForEach(ventaJson, json) {
        Venta nuevaVenta;
        nuevaVenta.venta_id = cJSON_GetObjectItem(ventaJson, "venta_id")->valueint;
        strcpy(nuevaVenta.fecha, cJSON_GetObjectItem(ventaJson, "fecha")->valuestring);
        nuevaVenta.producto_id = cJSON_GetObjectItem(ventaJson, "producto_id")->valueint;
        strcpy(nuevaVenta.producto_nombre, cJSON_GetObjectItem(ventaJson, "producto_nombre")->valuestring);
        strcpy(nuevaVenta.categoria, cJSON_GetObjectItem(ventaJson, "categoria")->valuestring);
        nuevaVenta.cantidad = cJSON_GetObjectItem(ventaJson, "cantidad")->valueint;
        nuevaVenta.precio_unitario = cJSON_GetObjectItem(ventaJson, "precio_unitario")->valuedouble;
        nuevaVenta.total = cJSON_GetObjectItem(ventaJson, "total")->valuedouble;

        agregarVenta(listaVentas, nuevaVenta);
    }

    // Liberar recursos
    cJSON_Delete(json);
    free(buffer);
    return 1;
}
