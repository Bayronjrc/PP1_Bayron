#include <stdio.h>
#include <stdlib.h>
#include "importacion.h"
#include "json_utils.h"
#include "data_structures.h"

// Función para importar datos desde un archivo JSON
void importarDatos() {
    char* rutaArchivo;
    rutaArchivo = (char*)malloc(256 * sizeof(char));
    

    printf("Ingrese la ruta del archivo JSON a importar: ");
    scanf("%s", rutaArchivo);

    // Lógica de importación usando json_utils
    cargarDatosJSON(&listaVentas, rutaArchivo);
    printf("Datos importados exitosamente. Total de ventas: %zu\n", listaVentas.tamano);

}
