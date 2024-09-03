#include <stdio.h>
#include <stdlib.h>
#include "importacion.h"
#include "data_processing.h"
#include "analisis.h"
#include "estadisticas.h"

#define NOMBRE_ARCHIVO "datos.json"

ListaVentas listaVentas;

void mostrarMenu();

int main() {
    int opcion;
    listaVentas.ventas = NULL;
    listaVentas.tamano = 0;
    listaVentas.capacidad = 0;
    // Cargar datos previos
    //cargarDatos();

    do {
        mostrarMenu();
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        switch(opcion) {
            case 1:
                importarDatos();
                imprimirListaVentas(&listaVentas);
                break;
            case 2:
                printf("\n--- Procesamiento de Datos ---\n");
                completarDatosFaltantes(&listaVentas);
                eliminarDuplicados(&listaVentas);
                break;
            case 3:
                printf("\n--- Analisis de Datos ---\n");
                printf("Total de ventas: %.2f\n", calcularTotalVentas(&listaVentas));
                calcularVentasMensualesAnuales(&listaVentas);
                break;
            case 4:
                printf("Realizando analisis temporal...\n");
                mesConMayorVenta(&listaVentas);
                diasSemanaMasActivo(&listaVentas);
                printf("Calcular tasa de crecimiento...\n");
                calcularTasaCrecimiento(2, 2023, 1, 2023, &listaVentas); 
                printf("Analisis temporal completado.\n");
                break;
            case 5:
                mostrarEstadisticas(&listaVentas);
                break;
            case 6:
                printf("Guardando datos y saliendo...\n");
                guardarDatosJSON(&listaVentas, NOMBRE_ARCHIVO);
                liberarListaVentas(&listaVentas);
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while(opcion != 6);

    return 0;
}

void mostrarMenu() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Importacion de datos\n");
    printf("2. Procesamiento de datos\n");
    printf("3. Analisis de datos\n");
    printf("4. Analisis temporal\n");
    printf("5. Estadisticas\n");
    printf("6. Salir\n");
}
