#include <stdio.h>
#include <stdlib.h>
#include "importacion.h"
#include "data_processing.h"
//#include "procesamiento.h"
//#include "analisis.h"
//#include "estadisticas.h"
//#include "persistence.h"

ListaVentas listaVentas;

void mostrarMenu();

int main() {
    int opcion;
    // Cargar datos previos
    //cargarDatos();

    do {
        mostrarMenu();
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        switch(opcion) {
            case 1:
                importarDatos();
                break;
            case 2:
                printf("\n--- Procesamiento de Datos ---\n");
                completarDatosFaltantes(&listaVentas);
                eliminarDuplicados(&listaVentas);
                break;
            case 3:
                //realizarAnalisis();
                break;
            case 4:
                //realizarAnalisisTemporal();
                break;
            case 5:
                //mostrarEstadisticas();
                break;
            case 6:
                liberarListaVentas(&listaVentas);
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while(opcion != 6);
//guardarDatos();

    return 0;
}

void mostrarMenu() {
    printf("\n--- Menú Principal ---\n");
    printf("1. Importación de datos\n");
    printf("2. Procesamiento de datos\n");
    printf("3. Análisis de datos\n");
    printf("4. Análisis temporal\n");
    printf("5. Estadísticas\n");
    printf("6. Salir\n");
}
